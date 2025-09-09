#include <cassert>
#include <stdexcept>

#include <mega/common/error_or.h>
#include <mega/log_level.h>
#include <mega/common/normalized_path.h>
#include <mega/common/task_queue.h>
#include <mega/fuse/common/client.h>
#include <mega/fuse/common/inode_info.h>
#include <mega/fuse/common/logging.h>
#include <mega/fuse/common/mount_event.h>
#include <mega/fuse/common/mount_event_type.h>
#include <mega/fuse/common/mount_info.h>
#include <mega/fuse/common/mount_result.h>
#include <mega/fuse/common/service.h>
#include <mega/fuse/platform/service_context.h>

namespace mega
{
namespace fuse
{

using namespace common;

Service::Service(Client& client, const ServiceFlags& flags)
  : mClient(client)
  , mContext()
  , mFlags(flags)
  , mFlagsLock()
{
    FUSEDebug1("Service constructed");
}

Service::Service(Client& client)
  : Service(client, ServiceFlags())
{
}

Service::~Service()
{
    FUSEDebug1("Service destroyed");
}

MountResult Service::add(const MountInfo& info)
{
    MountEvent event;

    event.mName = info.name();
    event.mResult = MOUNT_UNEXPECTED;
    event.mType = MOUNT_ADDED;

    if (mContext)
        event.mResult = mContext->add(info);

    emitEvent(mClient, event);

    return event.mResult;
}

bool Service::cached(NormalizedPath path) const
{
    return mContext && mContext->cached(path);
}

void Service::current()
{
    if (mContext)
        mContext->current();
}

ErrorOr<InodeInfo> Service::describe(const NormalizedPath& path) const
{
    if (mContext)
        return mContext->describe(path);

    return unexpected(API_ENOENT);
}

void Service::disable(MountDisabledCallback callback,
                      const std::string& name,
                      bool remember)
{
    assert(callback);

    if (mContext)
        return mContext->disable(std::move(callback),
                                 name,
                                 remember);

    MountEvent event;

    event.mName = name;
    event.mResult = MOUNT_UNKNOWN;
    event.mType = MOUNT_DISABLED;

    emitEvent(mClient, event);

    callback(event.mResult);
}

MountResult Service::discard(bool discard)
{
    if (mContext)
        return mContext->discard(discard);

    return MOUNT_UNEXPECTED;
}

void Service::deinitialize()
{
    mContext.reset();

    FUSEDebug1("Service deinitialized");
}

MountResult Service::downgrade(const NormalizedPath& path,
                               std::size_t target)
{
    if (mContext)
        return mContext->downgrade(path, target);

    return MOUNT_UNSUPPORTED;
}

MountResult Service::enable(const std::string& name,
                            bool remember)
{
    MountEvent event;

    event.mName = name;
    event.mResult = MOUNT_UNKNOWN;
    event.mType = MOUNT_ENABLED;

    if (mContext)
        event.mResult = mContext->enable(name, remember);

    if (event.mResult != MOUNT_SUCCESS)
        emitEvent(mClient, event);

    return event.mResult;
}

bool Service::enabled(const std::string& name) const
{
    return mContext && mContext->enabled(name);
}

Task Service::execute(std::function<void(const Task&)> function)
{
    if (mContext)
        return mContext->execute(std::move(function));

    Task task(std::move(function), logger());

    task.cancel();

    return task;
}

MountResult Service::flags(const std::string& name,
                           const MountFlags& flags)
{
    MountEvent event;

    event.mName = name;
    event.mResult = MOUNT_UNKNOWN;
    event.mType = MOUNT_CHANGED;

    if (mContext)
        event.mResult = mContext->flags(name, flags);

    emitEvent(mClient, event);

    return event.mResult;
}

MountFlagsPtr Service::flags(const std::string& name) const
{
    if (mContext)
        return mContext->flags(name);

    return nullptr;
}

MountInfoPtr Service::get(const std::string& name) const
{
    if (mContext)
        return mContext->get(name);

    return nullptr;
}

MountInfoVector Service::get(bool onlyEnabled) const
{
    if (mContext)
        return mContext->get(onlyEnabled);

    return MountInfoVector();
}

MountResult Service::initialize()
try
{
    mContext = std::make_unique<platform::ServiceContext>(ServiceFlags(), *this);

    FUSEDebug1("Service initialized");

    return MOUNT_SUCCESS;
}
catch (std::runtime_error& exception)
{
    FUSEErrorF("Unable to initialize service: %s", exception.what());

    return MOUNT_UNEXPECTED;
}

void Service::logLevel(LogLevel level)
{
    std::lock_guard<std::mutex> guard(mFlagsLock);

    mFlags.mLogLevel = level;

    logger().logLevel(level);
}

LogLevel Service::logLevel() const
{
    return logger().logLevel();
}

NormalizedPath Service::path(const std::string& name) const
{
    if (mContext)
        return mContext->path(name);

    return NormalizedPath();
}

MountResult Service::remove(const std::string& name)
{
    MountEvent event;

    event.mName = name;
    event.mResult = MOUNT_UNKNOWN;
    event.mType = MOUNT_REMOVED;

    if (mContext)
        event.mResult = mContext->remove(name);

    emitEvent(mClient, event);

    return event.mResult;
}

void Service::serviceFlags(const ServiceFlags& flags)
{
    std::lock_guard<std::mutex> guard(mFlagsLock);

    mFlags = flags;

    logger().logLevel(mFlags.mLogLevel);

    if (mContext)
        mContext->serviceFlags(mFlags);
}

ServiceFlags Service::serviceFlags() const
{
    std::lock_guard<std::mutex> guard(mFlagsLock);

    return mFlags;
}

void Service::updated(NodeEventQueue& events)
{
    if (mContext)
        mContext->updated(events);
}

bool Service::syncable(const NormalizedPath& path) const
{
    if (mContext)
        return mContext->syncable(path);

    return true;
}

MountResult Service::upgrade(const NormalizedPath& path,
                             std::size_t target)
{
    if (mContext)
        return mContext->upgrade(path, target);

    return MOUNT_UNSUPPORTED;
}

} // fuse
} // mega

