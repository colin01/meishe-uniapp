//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 18. 2022
//   Author:        Meishe video team
//================================================================================

#import <Foundation/Foundation.h>
#import "NvsAICommon.h"
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol NvsAIDecodeProtocol <NSObject>

- (BOOL)decode:(CMSampleBufferRef)sampleBuffer completion:(void(^)(CVPixelBufferRef _Nullable pixelBuffer))completion;

/// 释放解码器
- (void)asyncInvalidateWithCompletion:(void(^)(void))completion;

- (nullable NSString *)savePixelBufferToTemp:(CVPixelBufferRef)pixelBuffer;

@end



typedef NS_ENUM(int, NvsAIDetectErrorType) {
    NvsAIDetectErrorTypeNone = 0,                       //!< \if ENGLISH No error \else 无错误 \endif
    NvsAIDetectErrorTypeFaildLoadModelFailed,           //!< \if ENGLISH Failed to load model \else 加载模型失败 \endif
    NvsAIDetectErrorTypeFaildBuildInterpreterFailed,    //!< \if ENGLISH Failed to build interpreter \else 构建解释器失败 \endif
    NvsAIDetectErrorTypeFailedAllocateFailed,           //!< \if ENGLISH Failed to allocate \else 分配失败 \endif
    NvsAIDetectErrorTypeReadImageDataFailed,            //!< \if ENGLISH Failed to read image data \else 读取图像数据失败 \endif
    NvsAIDetectErrorTypeInferenceFailed,                //!< \if ENGLISH Inference failed \else 推理失败 \endif
    NvsAIDetectErrorTypeParamsConflict,                 //!< \if ENGLISH Parameters conflict \else 参数冲突 \endif
    NvsAIDetectErrorTypeCreateGpuDelegateFailed,        //!< \if ENGLISH Failed to create GPU delegate \else 创建GPU委托失败 \endif
    NvsAIDetectErrorTypeQueryTensorsInfoFailed,         //!< \if ENGLISH Failed to query tensors info \else 查询张量信息失败 \endif
    NvsAIDetectErrorTypeIO,                             //!< \if ENGLISH IO error \else IO错误 \endif
    NvsAIDetectErrorTypeIOPermission,                   //!< \if ENGLISH IO permission error \else IO权限错误 \endif
    NvsAIDetectErrorTypeHardwareVideoDecoding           //!< \if ENGLISH Hardware video decoding error \else 硬件视频解码错误 \endif
};

@protocol NvsAIDetectorDelegate <NSObject>

/*! \if ENGLISH
 *  \brief Start detecting.
 *  \param videoPath video path.
 *  \param taskId task ID.
 *  \else
 *  \brief 开始检测
 *  \param videoPath 视频路径
 *  \param taskId 任务序号
 *  \endif
 */
- (void)didDetectStarted:(NSString *)videoPath taskId:(int64_t)taskId;

/*! \if ENGLISH
 *  \brief Finish detecting.
 *  \param videoPath video path.
 *  \param scdPath Detection result file path
 *  \else
 *  \brief 完成检测
 *  \param videoPath 视频路径
 *  \param scdPath 检测结果文件路径
 *  \endif
 */
- (void)didDetectFinished:(NSString *)videoPath scdPath:(NSString *)scdPath;

/*! \if ENGLISH
*  \brief detect exception information.
*  \param errorType Type of detect error.
*  \else
*  \brief 检测异常信息
*  \param errorType 错误类型
*  \endif
*/
- (void)didDetectError:(NvsAIDetectErrorType)errorType;

/*! \if ENGLISH
*  \brief Progress of detect.
*  \param videoPath video path.
*  \param progress Progess value.
*  \else
*  \brief 检测进度
*  \param videoPath 视频路径
*  \param progress 进度值
*  \endif
*/
- (void)didDetectProgress:(NSString *)videoPath progress:(float)progress;

@end

@protocol NvsAIDetectorRecordDelegate <NSObject>

- (void)onMatchEventFinished:(NvsMatchEventRetInfo *)retInfo;
- (void)onMatchEventInfo:(NSString *)channel info:(NSString *)info;
- (void)onRemoveVideoGroup:(NSArray<NSString *> *)videoGroup;

@end

@protocol NvsAIDetectorAnalyzeFrameDelegate <NSObject>

- (void)onDeduplicationFinished:(NSArray<NvsDeduplicationInfo *> *)deduplicationInfos;
- (void)onMatchFrameHighlightFinished:(NSArray<NvsMatchFrameHighlightInfo *> *)matchFrameHighlightInfos;

@end

@protocol NvsAIDetectorCutHighlightDelegate <NSObject>

- (void)onCutFinished:(NSString *)mediaPath infodataPath:(NSString *)infodataPath modeType:(int)modeType;
- (void)onCutCompleted:(int)modeType;
- (void)onDeleteOriginAssets:(NSArray<NvsMediaInfoDataPath *> *)paths modeType:(int)modeType;

@end

typedef NS_ENUM(int, NvsAIDetectModelType) {
    NvsAIDetectModelTypeFace = 0,                   //!< \if ENGLISH Face detection model \else 人脸检测模型 ms_face_* \endif
    NvsAIDetectModelTypeFaceAttr,                   //!< \if ENGLISH Face attribute model \else 人脸属性模型 FaceAttri.model \endif
    NvsAIDetectModelTypeExpression,                 //!< \if ENGLISH Expression model \else 表情模型 \endif
    NvsAIDetectModelTypeSmile,                      //!< \if ENGLISH smile model \else 笑模型 \endif
    NvsAIDetectModelTypeSkySeg,                     //!< \if ENGLISH Sky segmentation model \else 天空分割模型 ms_skyseg_* \endif
    NvsAIDetectModelTypeSkyClass,                   //!< \if ENGLISH Sky classification model \else 天空分类模型 ms_skyclass_* \endif
    NvsAIDetectModelTypeActivity,                   //!< \if ENGLISH Activity classification model \else 活动分类模型 activity_* \endif
    NvsAIDetectModelTypeObjDet,                     //!< \if ENGLISH Object detection model \else 目标检测模型 ms_vehicle_pedestrian_* \endif
    NvsAIDetectModelTypeScene,                      //!< \if ENGLISH Scene classification model \else 景别分类模型 sf.model \endif
    NvsAIDetectModelTypeAesthetic,                  //!< \if ENGLISH Aesthetic model \else 美学模型 \endif
    NvsAIDetectModelTypeHandGesture,                //!< \if ENGLISH Hand gesture model \else 手势模型 \endif
    NvsAIDetectModelTypeNio4,                       //!< \if ENGLISH Nio4 model \else Nio4模型 \endif
    NvsAIDetectModelTypeSceneSide,                  //!< \if Side scene model \else 侧面场景模型 \endif
    NvsAIDetectModelTypeMainColor                   //!< \if ENGLISH Main color detection (no model needed, use "" as path) \else 主色检测 不需要模型，直接使用""作为路径 \endif
};

/// AI 检测事件类型枚举
typedef NS_ENUM(NSInteger, NvsAIDetectEventType) {
    /// 无检测事件
    NvsAIDetectEventTypeNone = -1,
    /// 街道/大道检测事件（对应原 BOULEVARD）
    NvsAIDetectEventTypeBoulevard = 0,
    /// 天空检测事件
    NvsAIDetectEventTypeSky,
    /// 云朵检测事件
    NvsAIDetectEventTypeCloud
};


/*! \if ENGLISH
*   \brief AI detection class
*   \warning In the NvsAIDetector class, all public APIs can be used not only in the UI thread, but also across threads! ! !
*   \else
*   \brief AI检测类
*   \warning NvsAIDetector类中API不仅可以在UI线程使用，也可以跨线程使用！！！
*   \endif
*/
NVS_EXPORT @interface NvsAIDetector : NvsObject

@property (nonatomic, weak) id<NvsAIDetectorDelegate> detectorDelegate;
//@property (nonatomic, weak) id<NvsAIDetectorRecordDelegate> recordDelegate;
//@property (nonatomic, weak) id<NvsAIDetectorAnalyzeFrameDelegate> analyzeFrameDelegate;
@property (nonatomic, weak) id<NvsAIDetectorCutHighlightDelegate> cutHighlightDelegate;

/*! \if ENGLISH
 *  \brief Initializes detector instance.
 *  \return Returns an object instance of the detector.
 *  \else
 *  \brief 获取AI检测实例
 *  \return 返回AI检测类的对象实例
 *  \endif
 *  \sa close
*/
- (instancetype)initWithFlags:(int)flags;

- (BOOL)initModel:(NSString *)modelPath type:(NvsAIDetectModelType)type;
- (BOOL)initModel:(NSString *)modelPath type:(NvsAIDetectModelType)type modelConfig:(NvsAIModelConfig *_Nullable)modelConfig;

- (void)setConfigPath:(NSString *)path;

- (BOOL)detectMediaFiles:(NSArray<NvsDetectorMediaInfo *> *)mediaFiles;
- (BOOL)detectMediaFiles:(NSArray<NvsDetectorMediaInfo *> *)mediaFiles
             extroConfig:(NvsExtraParmConfig *_Nullable)extroConfig;

- (BOOL)cutHighlights:(NvsCutHighlightInfo*)highlightInfo
          extraConfig:(NvsExtraParmConfig *)extraConfig;

- (BOOL)deduplicateAssets:(NSArray<NvsDetectorMediaInfo *> *)mediaFiles
                extroInfo:(NvsDeduplicationExtroInfo *_Nullable)extroInfo;

- (BOOL)matchFrameHighlight:(NSArray<NvsDetectorMediaInfo *> *)mediaFiles
                  extroInfo:(NvsMatchFrameHighlightExtroInfo *_Nullable)extroInfo;

- (void)cancelTask:(int64_t)taskId;
- (void)cancelAllTasks;
- (void)suspend;
- (void)resume;

- (void)setParserFrameSpace:(uint32_t)frameSpace;
- (void)cancelParser:(int64_t)taskId;
- (BOOL)detectAutoRecordResults:(NSString *)infodataPath
                     recordInfo:(NvsFileAutoRecordInfos *)recordInfo;

- (void)setParserSoftwareDecode:(BOOL)softwareDecode;

- (void)setRecordConfig:(NvsRecordConfig *)config;
- (NvsRecordConfig *_Nullable)getRecordConfig;

// start to record
- (void)startRecord:(NvsRecordInfo *)recordInfo;

- (BOOL)detectAITags:(NvsDetectorImage)img
      cameraPosition:(NSString *)cameraPosition
               vinfo:(NvsVehicleInfo *_Nullable)vinfo
            fakedate:(int64_t)fakedate;//  0

- (BOOL)detectAITagsWithPixelBuffer:(CVPixelBufferRef)pixelBuffer
                          timeStamp:(int64_t)timeStamp
                         frameIndex:(int64_t)frameIndex
                     cameraPosition:(NSString *)cameraPosition
                              vinfo:(NvsVehicleInfo *_Nullable)vinfo
                           fakedate:(int64_t)fakedate;

// stop record and save infodata
- (void)stopRecord:(NvsRecordSaveFile *)saveInfo
         extraInfo:(NvsVehicleExtraInfo *)extraInfo;

// stop task
- (void)stopTask;
- (void)cancelAllSessionDetect;


// Un-decoded buffer recording
- (void)startRecord:(NvsRecordInfo *)recordInfo decoder:(id<NvsAIDecodeProtocol> _Nullable)decoder;

- (BOOL)detectAITagsWithSampleBuffer:(CMSampleBufferRef)sampleBuffer
                           timeStamp:(int64_t)timeStamp
                           frameIndex:(int64_t)frameIndex
                      cameraPosition:(NSString *)cameraPosition
                               vinfo:(NvsVehicleInfo *_Nullable)vinfo
                            fakedate:(int64_t)fakedate;//  0

- (BOOL)fillDetectorImage:(NvsDetectorImage *)outImage
          withPixelBuffer:(CVPixelBufferRef)pixelBuffer
                timeStamp:(int64_t)timeStamp
               frameIndex:(int64_t)frameIndex;

- (void)stopRecord:(NvsRecordSaveFile *)saveInfo
         extraInfo:(NvsVehicleExtraInfo *)extraInfo
        completion:(void (^ __nullable)(void))completion;


- (void)invalidate;

@end

NS_ASSUME_NONNULL_END
