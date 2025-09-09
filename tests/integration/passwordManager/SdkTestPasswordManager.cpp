#include "SdkTestPasswordManager.h"

#include "mock_listeners.h"

#include <gmock/gmock.h>

using namespace testing;

void SdkTestPasswordManager::SetUp()
{
    SdkTest::SetUp();
    ASSERT_NO_FATAL_FAILURE(getAccountsForTest(1, true, MegaApi::CLIENT_TYPE_PASSWORD_MANAGER));
    ASSERT_NE(megaApi[0], nullptr);
    mApi = megaApi[0].get();
    ASSERT_NO_FATAL_FAILURE(initPasswordManagerBase());
    ASSERT_NO_FATAL_FAILURE(removePWMNodes());
}

void SdkTestPasswordManager::TearDown()
{
    removePWMNodes();
    SdkTest::TearDown();
}

void SdkTestPasswordManager::initPasswordManagerBase()
{
    testing::NiceMock<MockRequestListener> rl{mApi};
    const auto captureHandle = [this](const MegaRequest& req)
    {
        mPWMBaseNodeHandle = req.getNodeHandle();
    };
    rl.setErrorExpectations(API_OK,
                            _,
                            MegaRequest::TYPE_CREATE_PASSWORD_MANAGER_BASE,
                            std::move(captureHandle));
    RequestTracker rtPasswordManagerBase(megaApi[0].get());
    megaApi[0]->getPasswordManagerBase(&rl);
    ASSERT_TRUE(rl.waitForFinishOrTimeout(MAX_TIMEOUT));
    ASSERT_NE(mPWMBaseNodeHandle, UNDEF);
}

void SdkTestPasswordManager::removePWMNodes()
{
    const std::unique_ptr<MegaNode> pwmBaseNode(megaApi[0]->getNodeByHandle(mPWMBaseNodeHandle));
    ASSERT_TRUE(pwmBaseNode);
    ASSERT_NO_FATAL_FAILURE(purgeTree(0, pwmBaseNode.get(), false));
}
