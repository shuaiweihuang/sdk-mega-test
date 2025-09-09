/**
 * @file SdkTestPasswordManager_test.cpp
 * @brief This file defines some tests for testing password manager functionalities
 */

#include "integration/mock_listeners.h"
#include "megaapi.h"
#include "megautils.h"
#include "sdk_test_utils.h"
#include "SdkTestPasswordManager.h"

#include <gmock/gmock.h>

/**
 * @class SdkTestPasswordManagerImport
 * @brief Fixture for test suite to test password manager import functionality
 *
 */
class SdkTestPasswordManagerImport: public SdkTestPasswordManager
{
public:
    static constexpr auto MAX_TIMEOUT{3min};

    using BadEntries = std::map<std::string, int64_t>;
    using ImportPassFileResult = std::variant<long long, BadEntries>;

    /**
     * @brief Imports the passwords contained in the file at the given path
     *
     * @return std::variant that can be:
     * - long long: An error code different from API_OK. This means that the request didn't finish
     *   as expected. There are two specific error codes that are set by this wrapper method:
     *   + API_ETEMPUNAVAIL: When the request didn't finish on time
     *   + API_EKEY: Request finished with API_OK but the getMegaStringIntegerMap returned nullptr
     * - BadEntries: A map with the entries that were not properly parsed from the file and an
     *   associated error code.
     */
    ImportPassFileResult importPasswordsFromFile(const fs::path& filePath) const
    {
        testing::NiceMock<MockRequestListener> rl{megaApi[0].get()};
        ImportPassFileResult result{API_ETEMPUNAVAIL};
        EXPECT_CALL(rl, onRequestFinish)
            .WillOnce(
                [&result, &rl](MegaApi*, MegaRequest* req, MegaError* err)
                {
                    if (err->getErrorCode() == API_OK)
                    {
                        MegaStringIntegerMap* stringIntegerList = req->getMegaStringIntegerMap();
                        if (stringIntegerList)
                            result = stringIntegerMapToMap(*stringIntegerList);
                        else
                            result = API_EKEY;
                    }
                    else
                        result = err->getErrorCode();
                    rl.markAsFinished();
                });

        megaApi[0]->importPasswordsFromFile(filePath.u8string().c_str(),
                                            MegaApi::IMPORT_PASSWORD_SOURCE_GOOGLE,
                                            getBaseHandle(),
                                            &rl);
        rl.waitForFinishOrTimeout(MAX_TIMEOUT);
        return result;
    }

    /**
     * @brief Same as previous overload but taking a LocalTempFile
     */
    ImportPassFileResult importPasswordsFromFile(sdk_test::LocalTempFile&& file) const
    {
        return importPasswordsFromFile(file.getPath());
    }

    /**
     * @brief Returns a vector with the names of the password nodes hanging from the password
     * manager base node. In this context, these are the nodes that were successfully imported.
     */
    std::vector<std::string> getImportedPassNodesNames() const
    {
        std::unique_ptr<MegaNodeList> list{megaApi[0]->getChildren(getBaseNode().get())};
        if (!list)
            return {};
        return toNamesVector(*list);
    }
};

/**
 * @brief SdkTestPasswordManagerImport.SdkTestImportPasswordAllEntriesOk
 */
TEST_F(SdkTestPasswordManagerImport, SdkTestImportPasswordAllEntriesOk)
{
    LOG_debug << "# Create csv file";
    constexpr std::string_view fileContents{R"(name,url,username,password,note
foo.com,https://foo.com/,tx,"hola""""\""\"".,,",
hello.co,https://hello.co/,hello,hello.1234,Description with ñ
test.com,https://test.com/,test3,"hello.12,34",
test.com,https://test.com/,txema,hel\nlo.1234,""
test2.com,https://test2.com/,test,hello.1234,
)"};
    const auto fname = "test.csv";

    LOG_debug << "# Import google csv file";
    const auto badEntriesVar = importPasswordsFromFile({fname, fileContents});
    const auto badEntries = std::get_if<BadEntries>(&badEntriesVar);
    ASSERT_TRUE(badEntries) << "Something went wrong importing the file";
    ASSERT_TRUE(badEntries->empty());
    ASSERT_THAT(getImportedPassNodesNames(),
                testing::UnorderedElementsAre("foo.com",
                                              "hello.co",
                                              "test.com",
                                              "test.com (1)",
                                              "test2.com"));
}

/**
 * @brief SdkTestPasswordManagerImport.SdkTestImportPasswordFails
 *
 *  - Import file with invalid path
 *  - Import empty file
 */
TEST_F(SdkTestPasswordManagerImport, SdkTestImportPasswordFails)
{
    LOG_debug << "# Import google csv file - null path";
    ASSERT_EQ(importPasswordsFromFile(""), ImportPassFileResult{API_EREAD});

    LOG_debug << "# Import google csv file - empty file";
    const std::string fname = "test.csv";
    ASSERT_EQ(importPasswordsFromFile({fname, 0}), ImportPassFileResult{API_EACCESS});
}

/**
 * @brief SdkTestPasswordManagerImport.SdkTestImportPasswordSomeRowsWrong
 */
TEST_F(SdkTestPasswordManagerImport, SdkTestImportPasswordSomeRowsWrong)
{
    LOG_debug << "# Create csv file";
    constexpr std::string_view fileContents{R"(name,url,username,password,note
name,https://foo.com/,username,password,note
name2,https://foo.com/,username,,note
name3,username,password,note
,https://foo.com/,username,password,note
)"};
    const std::string fname = "test.csv";

    const auto badEntriesVar = importPasswordsFromFile({fname, fileContents});
    const auto badEntries = std::get_if<BadEntries>(&badEntriesVar);
    ASSERT_TRUE(badEntries) << "Something went wrong importing the file";
    ASSERT_EQ(*badEntries,
              (BadEntries{
                  {"name2,https://foo.com/,username,,note",
                   MegaApi::IMPORTED_PASSWORD_ERROR_MISSINGPASSWORD},
                  {"name3,username,password,note", MegaApi::IMPORTED_PASSWORD_ERROR_PARSER},
                  {",https://foo.com/,username,password,note",
                   MegaApi::IMPORTED_PASSWORD_ERROR_MISSINGNAME},
              }));
    ASSERT_THAT(getImportedPassNodesNames(), testing::UnorderedElementsAre("name"));
}

/**
 * @brief SdkTestPasswordManagerImport.SdkTestImportPasswordAllRowsWrong
 */
TEST_F(SdkTestPasswordManagerImport, SdkTestImportPasswordAllRowsWrong)
{
    LOG_debug << "# Create csv file";
    constexpr std::string_view fileContents{R"(name,url,username,password,note
name2,https://foo.com/,username,,note
name3,username,password,note
,https://foo.com/,username,password,note
)"};
    const std::string fname = "test.csv";

    const auto badEntriesVar = importPasswordsFromFile({fname, fileContents});
    const auto badEntries = std::get_if<BadEntries>(&badEntriesVar);
    ASSERT_TRUE(badEntries) << "Something went wrong importing the file";
    ASSERT_EQ(*badEntries,
              (BadEntries{
                  {"name2,https://foo.com/,username,,note",
                   MegaApi::IMPORTED_PASSWORD_ERROR_MISSINGPASSWORD},
                  {"name3,username,password,note", MegaApi::IMPORTED_PASSWORD_ERROR_PARSER},
                  {",https://foo.com/,username,password,note",
                   MegaApi::IMPORTED_PASSWORD_ERROR_MISSINGNAME},
              }));
    ASSERT_TRUE(getImportedPassNodesNames().empty());
}
