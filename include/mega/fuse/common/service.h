#pragma once

#include <chrono>
#include <cstddef>
#include <mutex>

#include <mega/common/client_forward.h>
#include <mega/common/error_or_forward.h>
#include <mega/log_level_forward.h>
#include <mega/common/node_event_queue_forward.h>
#include <mega/common/normalized_path_forward.h>
#include <mega/common/task_queue_forward.h>
#include <mega/fuse/common/inode_info_forward.h>
#include <mega/fuse/common/mount_flags_forward.h>
#include <mega/fuse/common/mount_info_forward.h>
#include <mega/fuse/common/mount_result_forward.h>
#include <mega/fuse/common/service_callbacks.h>
#include <mega/fuse/common/service_context_forward.h>
#include <mega/fuse/common/service_flags.h>
#include <mega/fuse/common/service_forward.h>

#include <mega/types.h>

namespace mega
{
namespace fuse
{

class Service final
{
public:
    Service(common::Client& client, const ServiceFlags& flags);

    explicit Service(common::Client& mClient);

    ~Service();

    // Abort (and unmount) any mounts matching predicate.
    static MountResult abort(AbortPredicate predicate);

    // Add a mount to the database.
    MountResult add(const MountInfo& info);

    // Check if a file exists in the cache.
    bool cached(common::NormalizedPath path) const;

    // Called by the client when its view of the cloud is current.
    void current();

    // Deinitialize the service.
    void deinitialize();

    // Describe the inode representing the file at the specified path.
    common::ErrorOr<InodeInfo> describe(const common::NormalizedPath& path) const;

    // Disable an enabled mount.
    void disable(MountDisabledCallback callback,
                 const std::string& name,
                 bool remember);

    // Discard node events.
    MountResult discard(bool discard);

    // Downgrade the FUSE database to the specified version.
    MountResult downgrade(const common::NormalizedPath& path, std::size_t target);

    // Enable a disabled mount.
    MountResult enable(const std::string& name, bool remember);

    // Query whether a specified mount is enabled.
    bool enabled(const std::string& name) const;

    // Execute a function on some thread.
    common::Task execute(std::function<void(const common::Task&)> function);

    // Update a mount's flags.
    MountResult flags(const std::string& name,
                      const MountFlags& flags);

    // Query a mount's flags.
    MountFlagsPtr flags(const std::string& name) const;

    // Describe the mount associated with path.
    MountInfoPtr get(const std::string& name) const;

    // Describe all (enabled) mounts.
    MountInfoVector get(bool onlyEnabled) const;

    // Initialize the service.
    MountResult initialize();

    // How verbose should our logging be?
    void logLevel(LogLevel level);

    // How verbose is our logging?
    LogLevel logLevel() const;

    // Retrieve the path the mount associated with name.
    common::NormalizedPath path(const std::string& name) const;

    // Remove a disabled mount from the database.
    MountResult remove(const std::string& name);

    // Update the service's flags.
    void serviceFlags(const ServiceFlags& flags);

    // Query the service's flags.
    ServiceFlags serviceFlags() const;

    // Is FUSE supported on this platform?
    bool supported() const;

    // Check whether the specified path is "syncable."
    //
    // A path is syncable if:
    // - It does not contain an active mount.
    // - It is not contained within an active mount.
    bool syncable(const common::NormalizedPath& path) const;

    // Called by the client when nodes have changed in the cloud.
    void updated(common::NodeEventQueue& events);

    // Update the FUSE database to the specified version.
    MountResult upgrade(const common::NormalizedPath& path, std::size_t target);

    // Who we call to learn about the cloud and transfer files.
    common::Client& mClient;

private:
    // Platform-specific behavior and state.
    ServiceContextPtr mContext;

    // Customizes how the service functions.
    ServiceFlags mFlags;

    // Serializes access to mFlags.
    mutable std::mutex mFlagsLock;
}; // Service

} // fuse
} // mega

