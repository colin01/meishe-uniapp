package com.malashequ.meishe;

import android.text.TextUtils;

import com.alibaba.fastjson.JSONObject;

import java.io.File;
import java.util.concurrent.atomic.AtomicBoolean;

import io.dcloud.feature.uniapp.bridge.UniJSCallback;

/** Coordinates the one native editor flow that the SDK can safely host at a time. */
public final class MeisheSessionCoordinator {
    private static final Object LOCK = new Object();
    private static Session activeSession;

    private MeisheSessionCoordinator() {
    }

    public static boolean start(String sessionId, UniJSCallback callback) {
        synchronized (LOCK) {
            if (activeSession != null && !activeSession.finished.get()) {
                return false;
            }
            activeSession = new Session(sessionId, callback);
            return true;
        }
    }

    public static void finishSuccess(String sessionId, String outputFilePath) {
        if (TextUtils.isEmpty(outputFilePath) || !new File(outputFilePath).isFile()) {
            finishFailure(sessionId, "output_missing", "视频导出文件不存在");
            return;
        }
        JSONObject result = new JSONObject();
        result.put("outputFilePath", outputFilePath);
        finish(sessionId, result);
    }

    public static void finishCancelled(String sessionId) {
        JSONObject result = new JSONObject();
        result.put("cancelled", true);
        finish(sessionId, result);
    }

    public static void finishFailure(String sessionId, String errorCode, String errorMessage) {
        JSONObject result = new JSONObject();
        result.put("errorCode", TextUtils.isEmpty(errorCode) ? "native_failed" : errorCode);
        result.put("errorMessage", TextUtils.isEmpty(errorMessage) ? "视频处理失败" : errorMessage);
        finish(sessionId, result);
    }

    private static void finish(String sessionId, JSONObject result) {
        Session session;
        synchronized (LOCK) {
            session = activeSession;
            if (session == null || !TextUtils.equals(session.sessionId, sessionId)
                    || !session.finished.compareAndSet(false, true)) {
                return;
            }
            activeSession = null;
        }
        result.put("sessionId", session.sessionId);
        try {
            if (session.callback != null) {
                session.callback.invoke(result);
            }
        } catch (Throwable ignore) {
            // JS runtime may already be destroyed. The session was still released exactly once.
        }
    }

    private static final class Session {
        final String sessionId;
        final UniJSCallback callback;
        final AtomicBoolean finished = new AtomicBoolean(false);

        Session(String sessionId, UniJSCallback callback) {
            this.sessionId = sessionId;
            this.callback = callback;
        }
    }
}
