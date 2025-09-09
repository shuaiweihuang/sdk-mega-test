/**
 * @file MEGATotpTokenGenResult.h
 * @brief Object Data for TOTP token generation result
 *
 * (c) 2025- by Mega Limited, Auckland, New Zealand
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

#import <Foundation/Foundation.h>
#import "MEGATotpTokenLifetime.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @brief Represents the result of generating a TOTP token, including a result code and token lifetime info.
 */
@interface MEGATotpTokenGenResult : NSObject

@property (nonatomic, assign) int errorCode;
@property (nonatomic, strong) MEGATotpTokenLifetime *result;

- (instancetype)initWithErrorCode:(int)errorCode result:(MEGATotpTokenLifetime *)result;

@end

NS_ASSUME_NONNULL_END
