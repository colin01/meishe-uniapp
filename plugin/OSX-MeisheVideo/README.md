# OSX-MeisheVideo

Android and iOS uni-app native plugin for the Meishe SDKDemo workflow. The module name stays
`AThree-meishe2` so existing application calls remain compatible.

## Methods

- `start(options, callback)`: Starts the video editing workflow by default. Set
  `mode: 'publish'` for one-click publishing: one video is selected, validated, copied to a
  readable local path, and returned without exporting a new video.
- `editor(options, callback)`: Compatibility entry that starts the video editing flow. Plugin
  sessions always expose `发布`, export a 720p video, and return `outputFilePath` through the
  same callback so the uni-app caller can open its video publish page directly.
- `shooting(options, callback)`: Returns `{ errorCode: 'unsupported' }` because
  this SDKDemo integration has no standalone capture implementation.

`maxTime` is optional and expressed in seconds. It is enforced for every plugin editing session.
When the
selected source exceeds the limit, the plugin keeps the selection session open and asks whether
to enter the editor: choosing `进入编辑` opens the existing 720p auto-ratio editing flow, while
choosing `重新选择` keeps the picker open without invoking the callback. Sources within the limit
continue to return their uploadable local path directly. Every terminal callback includes the
caller-provided `sessionId` or an internally generated one. Successful callbacks include
`outputFilePath`; cancellation is `{ cancelled: true }`; errors add `errorCode` and
`errorMessage`.

The exported file is checked against `maxTime` again before the success callback is sent. An
unreadable or still-over-limit result stays in the editor for further trimming; only a compliant result returns
`outputFilePath` to the application, which can then open its video publish page.

Android and iOS expose the same module name, methods, options, and callback contract. The iOS
package includes the arm64 static bridge, Meishe xcframework dependencies, and production
BundleResources including the production license bundle.

## Build

GitHub Actions builds all Android library AARs, the bundled Meishe SDK AARs, native libraries,
and `meishesdk.lic` into the plugin archive.
