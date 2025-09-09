/**
 * @file syncuploadthrottlingfile.h
 * @brief Class for UploadThrottlingFile.
 */

#ifndef MEGA_SYNCINTERNALS_UPLOADTHROTTLINGFILE_H
#define MEGA_SYNCINTERNALS_UPLOADTHROTTLINGFILE_H 1

#ifdef ENABLE_SYNC

#include "mega/file.h"

namespace mega
{

/**
 * @struct UploadThrottlingFile
 * @brief Handles upload throttling and abort handling for individual files.
 *
 * This struct encapsulates the logic for handling upload throttling and aborted uploads.
 * It tracks the number of uploads, manages timeouts, and
 * provides mechanisms for resetting counters and determining when throttling or upload
 * continuation should occur.
 *
 * @see UploadThrottlingManager
 */
struct UploadThrottlingFile
{
private:
    /**
     * @brief Counter for completed uploads.
     */
    unsigned mUploadCounter{};

    /**
     * @brief Timestamp of the last time the upload counter was processed.
     */
    std::chrono::steady_clock::time_point mUploadCounterLastTime{std::chrono::steady_clock::now()};

    /**
     * @brief Flag to bypass throttling logic.
     * This is meant for uncomplete uploads that were cancelled due to a change or failure.
     */
    bool mBypassThrottlingNextTime{};

public:
    /**
     * @brief Gets the mUploadCounter.
     */
    unsigned uploadCounter() const
    {
        return mUploadCounter;
    }

    /**
     * @brief Gets the mBypassThrottlingNextTime flag.
     */
    unsigned willBypassThrottlingNextTime() const
    {
        return mBypassThrottlingNextTime;
    }

    /**
     * @brief Increases the upload counter by 1.
     * Also checks if the upload counter is going to reach the max for its type.
     * In that case the upload counter is reset.
     */
    void increaseUploadCounter();

    /**
     * @brief Checks throttling control logic for uploads.
     *
     * Checks the bypassThrottlingNextTime flag in case it needs bypassing, and whether the number
     * of uploads exceeds the configured maximum before throttling, always that the time lapsed
     * since last upload counter processing does not exceed the
     * uploadCounterInactivityExpirationTime.
     *
     * @param uploadCounterInactivityExpirationTime Timeout for resetting the upload counter.
     * @return True if throttling must be applied, otherwise false.
     */
    bool checkUploadThrottling(const unsigned maxUploadsBeforeThrottle,
                               const std::chrono::seconds uploadCounterInactivityExpirationTime);

    /**
     * @brief Handles the logic for aborting uploads due to fingerprint mismatch or termination.
     *
     * The upload can only be aborted if:
     *   - The upload has already started (not in the throttling queue). Otherwise the
     * fingerprint of the upload is updated with the new one (unless the transfer direction needs to
     * change). No need to cancel the upload.
     *   - The upload has not started the putnodes request.
     *
     * If the above conditions are met the upload must be aborted.
     * Additionally, bypassThrottlingNextTime() is called in case that the upload must be
     * aborted and the transfer direction does not need to change.
     *
     * @return True if the upload should be aborted, otherwise false.
     */
    bool handleAbortUpload(SyncUpload_inClient& upload,
                           const bool transferDirectionNeedsToChange,
                           const FileFingerprint& fingerprint,
                           const unsigned maxUploadsBeforeThrottle,
                           const LocalPath& transferPath);

    /**
     * @brief Sets the mBypassThrottlingNextTime flag.
     *
     * The upload counter is not increased if the upload is not completed. However, the counter
     * could be greater than maxUploadsBeforeThrottle already, and the current upload has been
     * cancelled due to a fingerprint change or failure.
     * In that case, this method should be called to set the flag to true and bypass the throttling
     * logic upon the upload restart.
     */
    void bypassThrottlingNextTime(const unsigned maxUploadsBeforeThrottle);
};

} // namespace mega

#endif // ENABLE_SYNC
#endif // MEGA_SYNCINTERNALS_UPLOADTHROTTLINGFILE_H
