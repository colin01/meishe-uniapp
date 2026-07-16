//
//  NvCaptionTextView.h
//  NvShortVideoCore
//
//  Created by meishe on 2024/3/8.
//

#import <UIKit/UIKit.h>
#import "YYText.h"

NS_ASSUME_NONNULL_BEGIN

@interface NvCaptionEditViewTransInfo : NSObject

@property (nonatomic, assign) CGAffineTransform transform;
@property (nonatomic, assign) CGPoint center;

@end
@class NvImageCaptionModel;
@class NvCaptionViewTheme;
@interface NvCaptionTextView : YYTextView

//@property (nonatomic, assign) NvImageCaptionStyle imageCaptionStyle;
@property (nonatomic, assign) int imageCaptionStyle;
@property (nonatomic, strong) NSString *colorString;
@property (nonatomic, strong) NSString *inputFontName;

@property (nonatomic, assign) CGFloat viewWidth;
@property (nonatomic, assign) CGFloat viewHeight;
@property (nonatomic, strong, nullable) NvCaptionEditViewTransInfo *transInfo;

+ (instancetype)makeupTextView:(NvImageCaptionModel *)imageModel;

- (instancetype)initTextViewWithCenter:(CGPoint)center 
                             maxHeight:(CGFloat)maxHeight;

- (void)resetCenter:(CGPoint)center
          maxHeight:(CGFloat)maxHeight;

- (void)updateFontName:(NSString *_Nullable)fontName;

- (void)updateColor:(NSString *)colorString;

- (void)updateTextAlignment:(NSTextAlignment)alignment;

//- (void)updateCaptionStyle:(NvImageCaptionStyle)imageCaptionStyle;
- (void)updateCaptionStyle:(int)imageCaptionStyle;

- (void)updateTypingAttributes;

- (void)configViewText:(NSString *)text;

- (void)appleyTheme:(NvCaptionViewTheme *)theme;

@end

NS_ASSUME_NONNULL_END
