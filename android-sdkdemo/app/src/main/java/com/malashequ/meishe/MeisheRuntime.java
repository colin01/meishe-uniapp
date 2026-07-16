package com.malashequ.meishe;

import android.content.Context;

import com.meishe.sdkdemo.MSApplication;

public final class MeisheRuntime {
    private MeisheRuntime() {
    }

    public static void initialize(Context context) {
        if (context == null) {
            throw new IllegalStateException("APP 上下文不可用");
        }
        MSApplication.initialize(context.getApplicationContext());
    }
}
