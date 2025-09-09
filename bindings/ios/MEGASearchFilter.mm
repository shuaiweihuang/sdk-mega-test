/**
 * @file MEGASearchFilter.mm
 * @brief Class which encapsulates all data used for search nodes filtering
 *
 * (c) 2023- by Mega Limited, Auckland, New Zealand
 *
 * This file is part of the MEGA SDK - Client Access Engine.
 *
 * Applications using the MEGA API must present a valid application key
 * and comply with the the rules set forth in the Terms of Service.
 *
 * The MEGA SDK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @copyright Simplified (2-clause) BSD License.
 *
 * You should have received a copy of the license along with this
 * program.
 */
#import "MEGASearchFilter.h"
#import "MEGANode.h"

NS_ASSUME_NONNULL_BEGIN

@implementation MEGASearchFilter

- (instancetype)initWithTerm:(NSString *)term
                 description:(NSString * _Nullable)description
                         tag:(NSString * _Nullable)tag
            parentNodeHandle:(uint64_t)parentNodeHandle
                    nodeType:(MEGANodeType)nodeType
                    category:(MEGANodeFormatType)category
             sensitiveFilter:(MEGASearchFilterSensitiveOption)sensitiveFilter
             favouriteFilter:(MEGASearchFilterFavouriteOption)favouriteFilter
                locationType:(int)locationType
           creationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)creationTimeFrame
       modificationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)modificationTimeFrame
          useAndForTextQuery:(BOOL)useAndForTextQuery {
    self = [super init];

    if (self != nil) {
        _term = term;
        _searchDescription = description;
        _searchTag = tag;
        _parentNodeHandle = parentNodeHandle;
        _nodeType = nodeType;
        _category = category;
        _sensitiveFilter = sensitiveFilter;
        _favouriteFilter = favouriteFilter;
        _locationType = locationType;
        _creationTimeFrame = creationTimeFrame;
        _modificationTimeFrame = modificationTimeFrame;
        _useAndForTextQuery = useAndForTextQuery;
    }

    return self;
}

- (instancetype)initWithTerm:(NSString *)term
            parentNodeHandle:(uint64_t)parentNodeHandle
                    nodeType:(MEGANodeType)nodeType
                    category:(MEGANodeFormatType)category
             sensitiveFilter:(MEGASearchFilterSensitiveOption)sensitiveFilter
             favouriteFilter:(MEGASearchFilterFavouriteOption)favouriteFilter
           creationTimeFrame:(MEGASearchFilterTimeFrame* _Nullable)creationTimeFrame
       modificationTimeFrame:(MEGASearchFilterTimeFrame* _Nullable)modificationTimeFrame {
    return [self initWithTerm:term
                  description:nil
                          tag:nil
             parentNodeHandle:parentNodeHandle
                     nodeType:nodeType
                     category:category
              sensitiveFilter:sensitiveFilter
              favouriteFilter:favouriteFilter
                 locationType:-1
            creationTimeFrame:creationTimeFrame
        modificationTimeFrame:modificationTimeFrame
           useAndForTextQuery:NO];
}

- (instancetype)initWithTerm:(NSString *)term
                    nodeType:(MEGANodeType)nodeType
                    category:(MEGANodeFormatType)category
             sensitiveFilter:(MEGASearchFilterSensitiveOption)sensitiveFilter
             favouriteFilter:(MEGASearchFilterFavouriteOption)favouriteFilter
                locationType:(int)locationType
           creationTimeFrame:(MEGASearchFilterTimeFrame* _Nullable)creationTimeFrame
       modificationTimeFrame:(MEGASearchFilterTimeFrame* _Nullable)modificationTimeFrame {
    return [self initWithTerm:term
                  description:nil
                          tag:nil
             parentNodeHandle:-1
                     nodeType:nodeType
                     category:category
              sensitiveFilter:sensitiveFilter
              favouriteFilter:favouriteFilter
                 locationType:locationType
            creationTimeFrame:creationTimeFrame
        modificationTimeFrame:modificationTimeFrame
            useAndForTextQuery:NO];
}

- (instancetype)initWithTerm:(NSString *)term
                 description:(NSString * _Nullable)description
                         tag:(NSString * _Nullable)tag
            parentNodeHandle:(uint64_t)parentNodeHandle
                    nodeType:(MEGANodeType)nodeType
                    category:(MEGANodeFormatType)category
             sensitiveFilter:(MEGASearchFilterSensitiveOption)sensitiveFilter
             favouriteFilter:(MEGASearchFilterFavouriteOption)favouriteFilter
           creationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)creationTimeFrame
       modificationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)modificationTimeFrame
          useAndForTextQuery:(BOOL)useAndForTextQuery {
    return [self initWithTerm:term
            description:description
                          tag:tag
             parentNodeHandle:parentNodeHandle
                     nodeType:nodeType
                     category:category
              sensitiveFilter:sensitiveFilter
              favouriteFilter:favouriteFilter
                 locationType:-1
            creationTimeFrame:creationTimeFrame
            modificationTimeFrame:modificationTimeFrame
           useAndForTextQuery:useAndForTextQuery];
}

- (instancetype)initWithTerm:(NSString *)term
                 description:(NSString * _Nullable)description
                         tag:(NSString * _Nullable)tag
                    nodeType:(MEGANodeType)nodeType
                    category:(MEGANodeFormatType)category
             sensitiveFilter:(MEGASearchFilterSensitiveOption)sensitiveFilter
             favouriteFilter:(MEGASearchFilterFavouriteOption)favouriteFilter
                locationType:(int)locationType
           creationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)creationTimeFrame
       modificationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)modificationTimeFrame
          useAndForTextQuery:(BOOL)useAndForTextQuery {
    return [self initWithTerm:term
                  description:description
                          tag:tag
             parentNodeHandle:-1
                     nodeType:nodeType
                     category:category
              sensitiveFilter:sensitiveFilter
              favouriteFilter:favouriteFilter
                 locationType:locationType
            creationTimeFrame:creationTimeFrame
        modificationTimeFrame:modificationTimeFrame
            useAndForTextQuery:useAndForTextQuery];
}

- (instancetype)initWithTerm:(NSString *)term
                 description:(NSString * _Nullable)description
                         tag:(NSString * _Nullable)tag
            parentNodeHandle:(uint64_t)parentNodeHandle
                    nodeType:(int)nodeType
                    category:(int)category
                 sensitivity:(bool)sensitivity
             favouriteFilter:(int)favouriteFilter
           creationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)creationTimeFrame
       modificationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)modificationTimeFrame
          useAndForTextQuery:(BOOL)useAndForTextQuery {
    return [self initWithTerm:term
                  description:description
                          tag:tag
             parentNodeHandle:parentNodeHandle
                     nodeType:(MEGANodeType)nodeType
                     category:(MEGANodeFormatType)category
              sensitiveFilter:sensitivity? MEGASearchFilterSensitiveOptionNonSensitiveOnly : MEGASearchFilterSensitiveOptionDisabled
              favouriteFilter:(MEGASearchFilterFavouriteOption)favouriteFilter
                 locationType:-1
            creationTimeFrame:creationTimeFrame
            modificationTimeFrame:modificationTimeFrame
           useAndForTextQuery:useAndForTextQuery];
}

- (instancetype)initWithTerm:(NSString *)term
                 description:(NSString * _Nullable)description
                         tag:(NSString * _Nullable)tag
                    nodeType:(int)nodeType
                    category:(int)category
                 sensitivity:(bool)sensitivity
             favouriteFilter:(int)favouriteFilter
                locationType:(int)locationType
           creationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)creationTimeFrame
       modificationTimeFrame:(MEGASearchFilterTimeFrame * _Nullable)modificationTimeFrame
          useAndForTextQuery:(BOOL)useAndForTextQuery {
    return [self initWithTerm:term
                  description:description
                          tag:tag
             parentNodeHandle:-1
                     nodeType:(MEGANodeType)nodeType
                     category:(MEGANodeFormatType)category
              sensitiveFilter:sensitivity? MEGASearchFilterSensitiveOptionNonSensitiveOnly : MEGASearchFilterSensitiveOptionDisabled
              favouriteFilter:(MEGASearchFilterFavouriteOption)favouriteFilter
                 locationType:locationType
            creationTimeFrame:creationTimeFrame
        modificationTimeFrame:modificationTimeFrame
           useAndForTextQuery:useAndForTextQuery];
}

- (BOOL)didSetParentNodeHandle {
    return _parentNodeHandle != -1;
}

- (BOOL)didSetLocationType {
    return _locationType != -1;
}


@end

NS_ASSUME_NONNULL_END
