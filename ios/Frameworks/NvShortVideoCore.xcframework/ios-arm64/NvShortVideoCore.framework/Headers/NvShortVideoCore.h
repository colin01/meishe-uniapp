//
//  NvShortVideoCore.h
//  NvShortVideoCore
//
//  Created by chengww on 2022/1/4.
//

#import <Foundation/Foundation.h>

#define NvShortVideoCoreMajorVersion 2
#define NvShortVideoCoreMinorVersion 0
#define NvShortVideoCoreRevisionVersion 2
#define NvShortVideoCoreBuild 1

//! Project version number for NvShortVideoCore.
FOUNDATION_EXPORT double NvShortVideoCoreVersionNumber;

//! Project version string for NvShortVideoCore.
FOUNDATION_EXPORT const unsigned char NvShortVideoCoreVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <NvShortVideoCore/PublicHeader.h>
#if __has_include(<NvShortVideoCore/NCSequenceView.h>)
#import <NvShortVideoCore/NCSequenceView.h>
#import <NvShortVideoCore/NvAudioWaveView.h>
#import <NvShortVideoCore/NvWaveDataParse.h>
#import <NvShortVideoCore/NvSqlite3Operator.h>
#import <NvShortVideoCore/NvCaptionTextView.h>
#else
#import "NCSequenceView.h"
#import "NvAudioWaveView.h"
#import "NvWaveDataParse.h"
#import "NvSqlite3Operator.h"
#import "NvCaptionTextView.h"

#endif
/// -------YYTextView will be removed in a future version.-----------
#if __has_include(<YYText/YYText.h>)
FOUNDATION_EXPORT double YYTextVersionNumber;
FOUNDATION_EXPORT const unsigned char YYTextVersionString[];
#import <YYText/YYLabel.h>
#import <YYText/YYTextView.h>
#import <YYText/YYTextAttribute.h>
#import <YYText/YYTextArchiver.h>
#import <YYText/YYTextParser.h>
#import <YYText/YYTextRunDelegate.h>
#import <YYText/YYTextRubyAnnotation.h>
#import <YYText/YYTextLayout.h>
#import <YYText/YYTextLine.h>
#import <YYText/YYTextInput.h>
#import <YYText/YYTextDebugOption.h>
#import <YYText/YYTextKeyboardManager.h>
#import <YYText/YYTextUtilities.h>
#import <YYText/NSAttributedString+YYText.h>
#import <YYText/NSParagraphStyle+YYText.h>
#import <YYText/UIPasteboard+YYText.h>
#else
#import "YYLabel.h"
#import "YYTextView.h"
#import "YYTextAttribute.h"
#import "YYTextArchiver.h"
#import "YYTextParser.h"
#import "YYTextRunDelegate.h"
#import "YYTextRubyAnnotation.h"
#import "YYTextLayout.h"
#import "YYTextLine.h"
#import "YYTextInput.h"
#import "YYTextDebugOption.h"
#import "YYTextKeyboardManager.h"
#import "YYTextUtilities.h"
#import "NSAttributedString+YYText.h"
#import "NSParagraphStyle+YYText.h"
#import "UIPasteboard+YYText.h"
#endif
