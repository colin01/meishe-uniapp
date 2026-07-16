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
#import <NvStreamingSdkCore/NvsObject.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NvsDetectorParseMode) {
    NvsDetectorParseModePlayback = 0,
    NvsDetectorParseModeSeek
};

typedef NS_ENUM(NSInteger, NvsDetectorModeType) {
    NvsDetectorModeTypeNone = -1,
    NvsDetectorModeTypeCut = 0,
    NvsDetectorModeTypeOnly
};

typedef NS_ENUM(NSInteger, NvsDetectorImageFormat) {
    NvsDetectorImageFormatYUV420P = 0, // planar YUV 4:2:0, 12bpp
    NvsDetectorImageFormatNV12,    // planar YUV 4:2:0, 12bpp
    NvsDetectorImageFormatNV21,    // as above, but U and V bytes are swapped
    NvsDetectorImageFormatRGBA8,   // packed RGBA 8:8:8:8, 32bpp
    NvsDetectorImageFormatBGRA8,   // packed BGRA 8:8:8:8, 32bpp
};

typedef NS_ENUM(NSInteger, NvsDetectorYuvColorspace) {
    NvsDetectorYuvColorspaceInvalid = -1,
    NvsDetectorYuvColorspaceRec709 = 0,
    NvsDetectorYuvColorspaceRec601,
    NvsDetectorYuvColorspaceRec2020
};

typedef NS_ENUM(NSInteger, NvsDetectorYuvRange) {
    NvsDetectorYuvVideoRangeInvalid = -1,
    NvsDetectorYuvVideoRange = 0,
    NvsDetectorYuvFullRange
};

typedef struct NvsDetectorImage {
    void * _Nonnull data[4]; // plane's data pointer
    int pitch[4];  // plane's data stride
    NvsDetectorImageFormat format;
    uint32_t width;
    uint32_t height;
    uint32_t target_offset_x;
    uint32_t target_offset_y;
    uint32_t targetWidth;
    uint32_t targetHeight;
    int64_t timeStamp; // time in us, -1 means auto time
    int64_t frameIndex;
    NvsDetectorYuvColorspace colorspace;
    NvsDetectorYuvRange yuvRange;
} NvsDetectorImage;

NVS_EXPORT @interface NvsAIModelConfig : NSObject
@property (nonatomic, assign) int offset;
@property (nonatomic, assign) int interval;
@property (nonatomic, assign) int threadNum;
@property (nonatomic, assign) int detectFlag;
@end

NVS_EXPORT @interface NvsRecordSaveFile : NSObject

@property (nonatomic, strong) NSString *camera;
@property (nonatomic, strong) NSString *filePath;//with file name

@end

NVS_EXPORT @interface NvsRecordInfo : NSObject

@property (nonatomic, assign) BOOL isAutoRecord;
@property (nonatomic, assign) BOOL record;

@end

NVS_EXPORT @interface NvsRecordConfig : NSObject

@property (nonatomic, assign) uint64_t timeInterval;// = 5*60; // s
@property (nonatomic, assign) uint64_t mileageInterval;// = 5; // Km
@property (nonatomic, assign) uint64_t recordMinDuration;// = 10; // s
@property (nonatomic, assign) uint64_t recordMaxDuration;// = 20; // s

@end

NVS_EXPORT @interface NvsAutoRecordInfo : NSObject

@property (nonatomic, strong) NSString *eventName;
@property (nonatomic, assign) int64_t startRecord;
@property (nonatomic, assign) int64_t endRecord;

@end

NVS_EXPORT @interface NvsFileAutoRecordInfos : NSObject

@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, strong) NSArray<NvsAutoRecordInfo *> *recordInfos;

@end

NVS_EXPORT @interface NvsMatchFrameHighlightExtroInfo : NSObject
@end

NVS_EXPORT @interface NvsDeduplicationExtroInfo : NSObject
@end

NVS_EXPORT @interface NvsMediaProperty : NSObject

@property (nonatomic, strong) NSString *mediaPath;
@property (nonatomic, strong) NSString *infodataPath;
@property (nonatomic, assign) int64_t mediaBirthTimeStamp;
@property (nonatomic, assign) int64_t mediaDuration;

@end

NVS_EXPORT @interface NvsCutHighlightInfo : NSObject

@property (nonatomic, strong) NSArray<NvsMediaProperty *> *mediaProperties;
@property (nonatomic, strong) NSString *saveMediaDir;
@property (nonatomic, strong) NSString *saveInfodataDir;

@end

NVS_EXPORT @interface NvsFileNameConfig : NSObject

@property (nonatomic, assign) BOOL useDefaultName;
@property (nonatomic, assign) BOOL useTimeStamp;
@property (nonatomic, assign) BOOL useDuration;
@property (nonatomic, strong) NSString *flagStr;
@property (nonatomic, strong) NSString *connector;

@end

NVS_EXPORT @interface NvsExtraParmConfig : NSObject

@property (nonatomic, strong) NvsFileNameConfig *fileNameConfig;

@end

NVS_EXPORT @interface NvsVehiclePOI : NSObject

@property (nonatomic, strong) NSString *position;
@property (nonatomic, assign) float distance;

@end
    
typedef NS_ENUM(NSInteger, NvVehicleType) {
    NvVehicleTypeFuel     = 0, // Fuel vehicle / 油车（燃油车）
    NvVehicleTypeElectric = 1, // Electric vehicle / 电车（纯电）
    NvVehicleTypeHybrid   = 2  // Hybrid vehicle / 混动（混合动力）
};

typedef NS_ENUM(NSInteger, NvImageErrorType) {
    NvImageErrorTypeSuc     = 0,
    NvImageErrorTypeDelay   = 1
};


/// 视频创建类型（字符串枚举）
///
/// 用于区分视频创建 / 编辑的模式
typedef NSString * NvVideoCreateType NS_STRING_ENUM;

/// 智能创作 / Smart video creation
FOUNDATION_EXPORT NvVideoCreateType const NvVideoCreateTypeSmart;

/// 自由创作 / Free video creation
FOUNDATION_EXPORT NvVideoCreateType const NvVideoCreateTypeFree;

/// 通用创作 / Common video creation
FOUNDATION_EXPORT NvVideoCreateType const NvVideoCreateTypeCommon;


/// UAV 拍摄类型（字符串枚举）
///
/// 用于描述无人机的拍摄轨迹 / 视角类型
typedef NSString * UAVCaptureType NS_STRING_ENUM;

/// 无类型 / None
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeNone;

/// 后方拉远 / Pull back from behind
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypePullBack;

/// 远景环绕 / Far circle orbit
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeCircleFar;

/// 降临 / Approach (descending)
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeApproach;

/// 前方跟随 / Lead follow (front)
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeLead;

/// 主角视角 / Protagonist view
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeProtagonist;

/// 近景环绕 / Close circle orbit
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeCircleClose;

/// 近景跟随 / Close follow
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeClose;

/// 盘旋上升 / Hover and rise
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeHoverUp;

/// 扣拍旋转 / Hover and descend
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeHoverDown;

/// 冲天 / Skyward ascent
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeSkyward;

/// 顶部跟随 / Top follow
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeHead;

/// 云台仰角 / Gimbal look up
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeLookUp;

/// 空镜前飞 620 米 / Forward flight (620m)
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeForward620;

/// 后方跟随 / Follow from behind
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeBehind;

/// 后左方跟随 / Follow from behind-left
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeBehindLeft;

/// 左侧跟随 / Follow from left
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeLeft;

/// 左前方跟随 / Lead follow from left-front
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeLeadLeft;

/// 右前方跟随 / Lead follow from right-front
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeLeadRight;

/// 右侧跟随 / Follow from right
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeRight;

/// 右后方跟随 / Follow from behind-right
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeBehindRight;

/// 环绕类型 / Circle orbit
FOUNDATION_EXPORT UAVCaptureType const UAVCaptureTypeCircle;


NVS_EXPORT @interface NvsVehicleInfo : NSObject
// 基础运动参数
@property (nonatomic, assign) float speed;                  // 车速
@property (nonatomic, assign) float acceleration;           // 加速度
@property (nonatomic, assign) float accelerationVertical;   // 垂直加速度
@property (nonatomic, assign) float steeringAngle;          // 方向盘角度

//@property (nonatomic, assign) float temperature;            // 车外温度

@property (nonatomic, assign) float frontWheelAngle;        // 前轮转向
@property (nonatomic, assign) float backWheelAngle;         // 后轮转向
@property (nonatomic, assign) float headAngle;              // 航向角(指南针)

// 加速度计
@property (nonatomic, assign) float accelerationX;
@property (nonatomic, assign) float accelerationY;
@property (nonatomic, assign) float accelerationZ;

// 三轴角速度
@property (nonatomic, assign) float gyroscopeX;
@property (nonatomic, assign) float gyroscopeY;
@property (nonatomic, assign) float gyroscopeZ;


// 车辆控制参数
@property (nonatomic, assign) float acceleratorPosition;    // 加速踏板
@property (nonatomic, assign) float brakePedalPosition;     // 刹车踏板

// 位置信息
@property (nonatomic, assign) float longitude;              // 经度
@property (nonatomic, assign) float latitude;               // 纬度
@property (nonatomic, assign) float mileage;                // 里程
@property (nonatomic, strong) NSString *location;           // 地址信息

// 周边兴趣点， POI数组
@property (nonatomic, strong) NSArray<NvsVehiclePOI *> *poi;


/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 外部输入天气。
 *  @endif
 */
@property (nonatomic, assign) int apiWeather;

//@property (nonatomic, assign) int64_t fakeDate; // Fake date timestamp. 0

/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 发动机转速。
 *  @endif
 */
@property (nonatomic, assign) float rotateSpeed;

/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 已用电量。
 *  @endif
 */
@property (nonatomic, assign) float powerUsed;

/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 智能驾驶模式：雪地模式、越野模式
 *  @endif
 */
@property (nonatomic, strong) NSString *driveMode;

/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 剩余公里数。
 *  @endif
 */
@property (nonatomic, assign) float leftMileage;
@property (nonatomic, assign) float leftMileagePercent;

/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief uav参数为无机数据。
 *  @endif
 */
@property (nonatomic, assign) float uavSpeed; // 无人机水平速度
@property (nonatomic, assign) float uavSpeedVertical; // 无人机垂直速度
@property (nonatomic, assign) float uavSpeedMax; // 无人机最快水平速度
@property (nonatomic, assign) float uavSpeedVerticalMax; // 无人机最快垂直速度
@property (nonatomic, assign) float uavDistance; // 无人机水平离车距离
@property (nonatomic, assign) float uavDistanceVertical; // 无人机垂直离车距离
@property (nonatomic, assign) float uavDistanceVerticalMax; // 无人机最大垂直离车距离
@property (nonatomic, assign) float uavDuration; // 无人机飞行时长

// 无人机加速度计
@property (nonatomic, assign) float uavAccelerationX;
@property (nonatomic, assign) float uavAccelerationY;
@property (nonatomic, assign) float uavAccelerationZ;


@property (nonatomic, assign) float uavPoseYaw;     // 无人机姿态信息-偏航
@property (nonatomic, assign) float uavPosePitch;   // 无人机姿态信息-俯仰
@property (nonatomic, assign) float uavPoseRaw;     // 无人机姿态信息-横滚
@property (nonatomic, assign) float uavHeadingAngle; // 无人机航向角(指南针)

@property (nonatomic, strong) UAVCaptureType uavCaptureType; // 运镜

/*! @if ENGLISH
 *  @brief Overall vehicle output power
 *  @else
 *  @brief 整车输出功率。
 *  @endif
 */
@property (nonatomic, assign) float carPower;



/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 转向灯状态。
 *  @endif
 */
@property (nonatomic, assign) int turnState;
/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 车辆挡位。
 *  @endif
 */
@property (nonatomic, assign) int gear;

@property (nonatomic, assign) float cameraAngle; // 云台摄像头角度

@property (nonatomic, assign) NvImageErrorType imageErrorType; // 0 , 1

@property (nonatomic, strong) NvVideoCreateType videoCreateType; // 视频创建类型


// MARK: -- VehicleMotion

/*! @if ENGLISH
 *  @brief 0: Gasoline vehicle 1: Electric vehicle 2: Hybrid vehicle
 *  @else
 *  @brief 0:油车 1:电车 2:混动
 *  @endif
 */
//@property (nonatomic, assign) NvVehicleType carType;
//@property (nonatomic, strong) NSString *userName;       // 用户昵称
//@property (nonatomic, strong) NSString *driver;         // 主驾昵称
//@property (nonatomic, strong) NSString *codriver;       // 副驾昵称
//@property (nonatomic, strong) NSString *modelOfCar;     // 车辆型号

/*! @if ENGLISH
 *  @brief
 *  @else
 *  @brief 飞行器数据
 *  @endif
 */
@property (nonatomic, assign) float height;
@property (nonatomic, assign) float relativeHeight;
@property (nonatomic, assign) float speedVertical;
@property (nonatomic, assign) float speedHorizontal;
@property (nonatomic, assign) float rollAngle;
@property (nonatomic, assign) int phase;
// * VehicleInfo
@property (nonatomic, assign) int timeType;

@property (nonatomic, strong) NSString *landMarkName; //inductive reasoning by longitude,latitude and channel
@property (nonatomic, strong) NSString *country;
@property (nonatomic, strong) NSString *province;
@property (nonatomic, strong) NSString *city;
@property (nonatomic, strong) NSString *district;
@property (nonatomic, strong) NSString *street;

@property (nonatomic, assign) float shakeX;
@property (nonatomic, assign) float shakeY;
@property (nonatomic, assign) float shakeZ;

- (instancetype)init;

@end


NVS_EXPORT @interface NvsVehicleEffectInfo : NSObject

@property (nonatomic, strong) NSString *effectId;       //特效ID
@property (nonatomic, strong) NSString *effectType;     //特效类型

@end

NVS_EXPORT @interface NvsVehicleExtraInfo : NSObject

@property (nonatomic, assign) float temperature;    //车外温度
@property (nonatomic, strong) NSString *driver;     //主驾昵称
@property (nonatomic, strong) NSString *codriver;   //副驾昵称
@property (nonatomic, assign) float cameraAngle;    //里程
@property (nonatomic, assign) float odometer;
@property (nonatomic, strong) NSArray<NvsVehicleEffectInfo *> *effectInfos;
@property (nonatomic, assign) NvVehicleType carType;          // 0:油车 1:电车 2:混动
@property (nonatomic, strong) NSString *userName;   // 用户名称
@property (nonatomic, strong) NSString *modelOfCar; // 车辆型号

@end

NVS_EXPORT @interface NvsDetectFileInOut : NSObject

@property (nonatomic, assign) int64_t trimIn;
@property (nonatomic, assign) int64_t trimOut;

@end


NVS_EXPORT @interface NvsDetectorMediaInfo : NSObject

@property (nonatomic, assign) NvsDetectorModeType detectModeType;
@property (nonatomic, assign) NvsDetectorParseMode parseMode;
@property (nonatomic, strong) NSString *mediaPath;
@property (nonatomic, strong) NSString *mediaInfoPath;
@property (nonatomic, assign) BOOL parseKeyFrame;
@property (nonatomic, assign) uint32_t frameSpace;
@property (nonatomic, assign) int64_t seekInterval;//seek interval
@property (nonatomic, strong) NSString *camera;
@property (nonatomic, assign) uint64_t mediaBirthTimeStamp;//ms
@property (nonatomic, assign) uint64_t mediaDuration;//us
//used in detectOnly mode
@property (nonatomic, strong) NSString *saveInfoDataPath;
@property (nonatomic, strong) NSArray<NvsVehicleInfo *> *vehicleInfo;

@property (nonatomic, strong) NvsVehicleExtraInfo *vehicleExtraInfo;
@property (nonatomic, strong) NSArray<NvsDetectFileInOut *> *trimInOutList;

//used in detectWith Cut
@property (nonatomic, strong) NSString *saveMediaDir;
@property (nonatomic, strong) NSString *saveInfodataDir;
    
@property (nonatomic, assign) int64_t detectDurationAdded;

@end

NVS_EXPORT @interface NvsMatchEventRetInfo : NSObject
@property (nonatomic, strong) NSString *channel;
@property (nonatomic, assign) BOOL record;
@property (nonatomic, assign) int64_t timeStamp; //us
@property (nonatomic, strong) NSString *eventName;
@property (nonatomic, strong) NSString *imageColor;
@end

NVS_EXPORT @interface NvsMediaInfoDataPath : NSObject
@property (nonatomic, strong) NSString *mediaPath;
@property (nonatomic, strong) NSString *infoDataPath;
@end

NVS_EXPORT @interface NvsDeduplicationInfo : NSObject
@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, assign) float aesScore;
@end

NVS_EXPORT @interface NvsFrameHighlightInfo : NSObject
@property (nonatomic, assign) int64_t timeStamp;
@property (nonatomic, strong) NSString *eventName;
@property (nonatomic, assign) float eventScore;
@property (nonatomic, assign) float aesScore;
@end

NVS_EXPORT @interface NvsMatchFrameHighlightInfo : NSObject
@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, strong) NSArray<NvsFrameHighlightInfo *> *highlightInfos;
@end


// MARK: -- AI Info Helper Related Data Classes

typedef NS_ENUM(NSInteger, NvsCutSameResult) {
    NvsCutSameResultSucc = 0,
    NvsCutSameResultLessDuration,
    NvsCutSameResultTemplateLoss,
    NvsCutSameResultInfodataReadError,
    NvsCutSameResultInputVideoNumLessThanSlotNum
};

NVS_EXPORT @interface NvsMatchMemoryConfig : NSObject
@property (nonatomic, assign) uint64_t preferDate; // in ms
@property (nonatomic, assign) int memoryType;
@property (nonatomic, strong) NSString *defaultCity;
@end

NVS_EXPORT @interface NvsExtroMatchInfo : NSObject
@property (nonatomic, strong) NSArray<NSString *> *preferNames;
@property (nonatomic, strong) NSArray<NSString *> *blankAssets;
@property (nonatomic, strong) NSString * _Nullable destTemplateRatio;
@end

NVS_EXPORT @interface NvsCaptureVideoInfo : NSObject

@property (nonatomic, strong) NSString *camera;
@property (nonatomic, assign) int captureIndex;
@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, strong) NSString *infodataPath;
@property (nonatomic, strong) NSString *captureType;
@property (nonatomic, assign) int64_t    duration;
@property (nonatomic, assign) BOOL isVideo;

@end

NVS_EXPORT @interface NvsCaptureSame : NSObject
@property (nonatomic, strong) NSArray<NvsCaptureVideoInfo *> *videos;
@property (nonatomic, strong) NSString *infoJsonPath;
@end

NVS_EXPORT @interface NvsTimeStampValue : NSObject

@property (nonatomic, assign) int64_t timeStamp;
@property (nonatomic, assign) float value;
@property (nonatomic, strong) NSString *_Nullable strValue;

@end

NVS_EXPORT @interface NvsAICutterTag : NSObject

@property (nonatomic, strong) NSString *title;
@property (nonatomic, strong) NSString *memoryTitle;
@property (nonatomic, strong) NSString *userName;
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *tag;

@end

NVS_EXPORT @interface NvsUnitClip : NSObject
@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, strong) NSString *footageId;

@property (nonatomic, assign) size_t fileIdx;
@property (nonatomic, assign) int64_t start;
@property (nonatomic, assign) int64_t end;
@property (nonatomic, strong) NSDictionary<NSString *, NSArray<NvsTimeStampValue *> *> *formulaValues;
@property (nonatomic, assign) float score;
@property (nonatomic, assign) BOOL isAIGC;
@property (nonatomic, assign) int aigcType;
@property (nonatomic, assign) int aigcAlign;
@property (nonatomic, strong) NSString *aigcDesc;
@property (nonatomic, assign) BOOL isAigcBlank;
@property (nonatomic, assign) BOOL segable;
@property (nonatomic, assign) BOOL slotMatched;
@property (nonatomic, assign) int64_t videoDuration;

@end

NVS_EXPORT @interface NvsTemplateCaption : NSObject
@property (nonatomic, assign) size_t clipIndex;
@property (nonatomic, assign) size_t groupIndex;
@property (nonatomic, assign) int64_t inPoint;
@property (nonatomic, assign) int64_t duration;
@property (nonatomic, strong) NSString *defaultText;
@property (nonatomic, strong) NSString *defaultID;
@property (nonatomic, strong) NSString *formulaText;
@property (nonatomic, strong) NSString *targetText;
@end

NVS_EXPORT @interface NvsClipInfo : NSObject
@property (nonatomic, strong) NSString *templateId;
@property (nonatomic, assign) int templateType;
@property (nonatomic, strong) NSString *templateTypeDesc;
@property (nonatomic, strong) NSString *templageLabel;
@property (nonatomic, strong) NSArray<NSArray<NvsUnitClip *>*> *clips;
@property (nonatomic, strong) NSArray<NvsTemplateCaption *> *captions;
@property (nonatomic, strong) NSDictionary<NSString *, NSArray<NvsTimeStampValue *> *> *formulaValues;
@property (nonatomic, strong) NvsAICutterTag *tag;
@property (nonatomic, assign) float score;
@property (nonatomic, strong) NSArray<NSString *> *videoFxs;
@property (nonatomic, assign) int problem;
@property (nonatomic, assign) NvsCutSameResult result;

@end

NVS_EXPORT @interface NvsVideoInfo: NSObject
@property (nonatomic, strong) NSString *infoFilePath;
@property (nonatomic, assign) uint64_t start;
@property (nonatomic, assign) uint64_t end;
@property (nonatomic, strong) NSString *footageId;
@property (nonatomic, strong) NSString *videoPath;
@property (nonatomic, assign) int64_t videoDuration; // us
@property (nonatomic, assign) int assetWidth;
@property (nonatomic, assign) int assetHeight;
@end

NVS_EXPORT @interface NvsAICutFormulaDiscreteMap : NSObject
@property (nonatomic, assign) double minX;
@property (nonatomic, assign) double maxX;
@property (nonatomic, strong) NSString *eval;
@property (nonatomic, strong) NSArray<NSNumber *> *range; // for fix-int types mapping
@end

NVS_EXPORT @interface NvsAICutFormula : NSObject
@property (nonatomic, strong) NSString *formulaId;
@property (nonatomic, strong) NSString *func;
@property (nonatomic, assign) int shift;
@property (nonatomic, strong) NSArray<NvsAICutFormulaDiscreteMap *> *discreteMapFunc;
@property (nonatomic, strong) NSString *prefix;
@property (nonatomic, strong) NSString *suffix;
@property (nonatomic, assign) size_t keep; //how many num will be keep after decimal point
@property (nonatomic, strong) NSArray<NSString *> *args;
@property (nonatomic, assign) int64_t position; //used for memory caption
@property (nonatomic, assign) float shiftSpeed;
@end


NVS_EXPORT @interface NvsRequireClipInfo : NSObject

@property (nonatomic, strong) NSString * footageId;
@property (nonatomic, strong) NSString * camera;
@property (nonatomic, strong) NSString * format;
@property (nonatomic, assign) int64_t duration; // us
@property (nonatomic, assign) int64_t minDuration; // us
@property (nonatomic, assign) int captureIndex;
@property (nonatomic, assign) float multiple;

@end

NVS_EXPORT @interface NvsSubRequirement : NSObject

@property (nonatomic, strong) NSString * type;
@property (nonatomic, strong) NSString * editType;
@property (nonatomic, strong) NSString * category;
@property (nonatomic, strong) NSString * subcategory;
@property (nonatomic, assign) int clipCount;
@property (nonatomic, assign) int minClipCount;
@property (nonatomic, assign) int matchMinSlotCount;
@property (nonatomic, strong) NSArray<NvsRequireClipInfo *> *clipInfos;

@end


// Data class for AI task event
NVS_EXPORT @interface NvsContinuous2Int : NSObject

@property (nonatomic, assign) double minX;
@property (nonatomic, assign) double maxX;
@property (nonatomic, assign) int value;

@end

// Data class for AI task event
NVS_EXPORT @interface NvsAICutTag : NSObject

@property (nonatomic, strong) NSString *type;
@property (nonatomic, strong) NSArray<NSString *> *sumTags;
@property (nonatomic, strong) NSArray<NvsContinuous2Int *> *c2iMap;
@property (nonatomic, strong) NSArray<NSNumber *> *values;
@property (nonatomic, strong) NSArray<NSString *> *strValues;
@property (nonatomic, assign) float score;
@property (nonatomic, assign) float probThresh;

@end


// MARK: -- AI Info Helper

NVS_EXPORT @interface NvsAIFrameTag : NSObject

@property (nonatomic, strong) NSString *filePath;
@property (nonatomic, strong) NSString *prefer;
@property (nonatomic, strong) NSArray<NvsTimeStampValue *> *tags;

@end

// MARK: -- AI Edit Context Related Data Classes

// Event type enumeration
typedef NS_ENUM(int, NvsAIEventType) {
    NvsAIEventTypeCommon = 0,
    NvsAIEventTypeShot,
    NvsAIEventTypeMatch
};

// Data class for AI task event
NVS_EXPORT @interface NvsAITaskEvent : NSObject

@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *displayName;
@property (nonatomic, assign) NvsAIEventType eventType;
@property (nonatomic, strong) NSArray<NvsAICutTag *> *eventTags;
@property (nonatomic, strong) NSArray<NvsAICutTag *> *excludeTags;
@property (nonatomic, assign) float scoreMin;  

@end

// Data class for landmark information
NVS_EXPORT @interface NvsLandMark : NSObject

@property (nonatomic, strong) NSString *camera;
@property (nonatomic, assign) float longitudeMin;
@property (nonatomic, assign) float longitudeMax;
@property (nonatomic, assign) float latitudeMin;
@property (nonatomic, assign) float latitudeMax;
@property (nonatomic, assign) float headAngle;

@end

NVS_EXPORT @interface NvsLandMarkInfo : NSObject
@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSArray<NvsLandMark *> *info;
@end

// Data class for AIGC caption tag map
NVS_EXPORT @interface NvsAIGCCaptionTagMap : NSObject

@property (nonatomic, strong) NvsAICutTag *aiTag;
@property (nonatomic, strong) NSString *tagNumber;

@end

// Data class for AIGC caption
NVS_EXPORT @interface NvsAIGCCaption : NSObject

@property (nonatomic, strong) NSString *tagNumber;
@property (nonatomic, strong) NSString *type;
@property (nonatomic, strong) NSArray<NSString *> *captions;

@end


NS_ASSUME_NONNULL_END
