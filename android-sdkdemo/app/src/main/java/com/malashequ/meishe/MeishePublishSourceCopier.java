package com.malashequ.meishe;

import android.content.Context;
import android.net.Uri;
import android.text.TextUtils;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/** Makes scoped-storage album media available to the uni-app upload layer as a local file. */
public final class MeishePublishSourceCopier {
    private static final int BUFFER_SIZE = 32 * 1024;

    private MeishePublishSourceCopier() {
    }

    public static File prepare(Context context, String sourcePath, String displayName) throws IOException {
        if (context == null || TextUtils.isEmpty(sourcePath)) {
            throw new IOException("选中的视频路径为空");
        }

        if (!sourcePath.startsWith("content://")) {
            File sourceFile = new File(sourcePath);
            if (!sourceFile.isFile()) {
                throw new IOException("选中的视频文件不存在");
            }
            return sourceFile;
        }

        File targetDirectory = new File(context.getCacheDir(), "osx-meishe/publish");
        if (!targetDirectory.exists() && !targetDirectory.mkdirs()) {
            throw new IOException("无法创建视频缓存目录");
        }

        File targetFile = File.createTempFile("publish_", extensionOf(displayName), targetDirectory);
        InputStream sourceInput = context.getContentResolver()
                .openInputStream(Uri.parse(sourcePath));
        if (sourceInput == null) {
            //noinspection ResultOfMethodCallIgnored
            targetFile.delete();
            throw new IOException("无法读取选中的视频");
        }

        boolean copied = false;
        try (InputStream input = new BufferedInputStream(sourceInput);
             OutputStream output = new BufferedOutputStream(new FileOutputStream(targetFile))) {
            byte[] buffer = new byte[BUFFER_SIZE];
            int count;
            while ((count = input.read(buffer)) != -1) {
                output.write(buffer, 0, count);
            }
            output.flush();
            copied = targetFile.isFile() && targetFile.length() > 0;
            if (!copied) {
                throw new IOException("选中的视频复制失败");
            }
            return targetFile;
        } finally {
            if (!copied && targetFile.exists()) {
                //noinspection ResultOfMethodCallIgnored
                targetFile.delete();
            }
        }
    }

    private static String extensionOf(String displayName) {
        if (!TextUtils.isEmpty(displayName)) {
            int dotIndex = displayName.lastIndexOf('.');
            if (dotIndex >= 0 && dotIndex < displayName.length() - 1) {
                String extension = displayName.substring(dotIndex);
                if (extension.length() <= 10) {
                    return extension;
                }
            }
        }
        return ".mp4";
    }
}
