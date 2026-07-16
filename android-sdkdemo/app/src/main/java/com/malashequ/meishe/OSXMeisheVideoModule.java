package com.malashequ.meishe;

import android.content.Context;
import android.content.Intent;
import android.text.TextUtils;

import com.alibaba.fastjson.JSONObject;
import com.meishe.sdkdemo.selectmedia.SelectMediaActivity;
import com.meishe.sdkdemo.utils.Constants;
import com.meishe.sdkdemo.utils.MediaConstant;

import java.util.UUID;

import io.dcloud.feature.uniapp.annotation.UniJSMethod;
import io.dcloud.feature.uniapp.bridge.UniJSCallback;
import io.dcloud.feature.uniapp.common.UniModule;

public class OSXMeisheVideoModule extends UniModule {
    @UniJSMethod(uiThread = true)
    public void start(JSONObject options, UniJSCallback callback) {
        open(options, callback, null);
    }

    @UniJSMethod(uiThread = true)
    public void editor(JSONObject options, UniJSCallback callback) {
        open(options, callback, MeisheResultContract.MODE_EDIT);
    }

    @UniJSMethod(uiThread = true)
    public void shooting(JSONObject options, UniJSCallback callback) {
        invokeFailure(callback, "unsupported", "当前插件未提供独立拍摄入口");
    }

    private void open(JSONObject options, UniJSCallback callback, String forcedMode) {
        Context context = getModuleContext();
        if (context == null) {
            invokeFailure(callback, "context_unavailable", "APP 上下文不可用");
            return;
        }
        if (options == null || !options.containsKey("maxTime")
                || (forcedMode == null
                && !MeisheResultContract.MODE_PUBLISH.equals(options.getString("mode")))) {
            invokeFailure(callback, "invalid_options",
                    "调用必须传入 maxTime；start 调用还必须传入 mode: 'publish'");
            return;
        }

        String sessionId = options == null ? null : options.getString("sessionId");
        if (TextUtils.isEmpty(sessionId)) {
            sessionId = UUID.randomUUID().toString();
        }
        if (!MeisheSessionCoordinator.start(sessionId, callback)) {
            invokeFailure(callback, "busy", "已有视频编辑任务正在进行");
            return;
        }

        try {
            MeisheRuntime.initialize(context);
            Intent intent = new Intent(context, SelectMediaActivity.class);
            String mode = forcedMode == null ? (options == null ? null : options.getString("mode")) : forcedMode;
            if (!MeisheResultContract.MODE_PUBLISH.equals(mode)) {
                mode = MeisheResultContract.MODE_EDIT;
            }
            Integer maxTime = options == null ? null : options.getInteger("maxTime");
            int maxTimeSeconds = maxTime == null ? 0 : Math.max(0, maxTime);
            intent.putExtra(MeisheResultContract.EXTRA_SESSION_ID, sessionId);
            intent.putExtra(MeisheResultContract.EXTRA_MODE, mode);
            intent.putExtra(MeisheResultContract.EXTRA_MAX_TIME_SECONDS, maxTimeSeconds);
            intent.putExtra(MeisheResultContract.EXTRA_AUTO_EXPORT_720P, true);
            intent.putExtra("visitMethod", Constants.FROMMAINACTIVITYTOVISIT);
            intent.putExtra(MediaConstant.MEDIA_TYPE, MediaConstant.VIDEO);
            if (MeisheResultContract.MODE_PUBLISH.equals(mode)) {
                intent.putExtra("limitMediaCount", 1);
            }
            if (!(context instanceof android.app.Activity)) {
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            }
            context.startActivity(intent);
        } catch (Throwable error) {
            MeisheSessionCoordinator.finishFailure(sessionId, "launch_failed",
                    error.getMessage() == null ? "无法打开视频编辑器" : error.getMessage());
        }
    }

    private Context getModuleContext() {
        Object instance = getUniSdkInstanceCompat();
        if (instance == null) {
            return null;
        }
        try {
            java.lang.reflect.Method method = instance.getClass().getMethod("getContext");
            Object context = method.invoke(instance);
            return context instanceof Context ? (Context) context : null;
        } catch (Throwable ignore) {
            return null;
        }
    }

    private Object getUniSdkInstanceCompat() {
        try {
            java.lang.reflect.Method method = getClass().getMethod("getUniSDKInstance");
            Object instance = method.invoke(this);
            if (instance != null) return instance;
        } catch (Throwable ignore) {
        }
        Class<?> type = getClass();
        while (type != null) {
            try {
                java.lang.reflect.Field field = type.getDeclaredField("mUniSDKInstance");
                field.setAccessible(true);
                return field.get(this);
            } catch (NoSuchFieldException error) {
                type = type.getSuperclass();
            } catch (Throwable ignore) {
                return null;
            }
        }
        return null;
    }

    private void invokeFailure(UniJSCallback callback, String code, String message) {
        if (callback == null) return;
        JSONObject result = new JSONObject();
        result.put("errorCode", code);
        result.put("errorMessage", message);
        callback.invoke(result);
    }
}
