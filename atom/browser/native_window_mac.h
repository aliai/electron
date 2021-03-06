// Copyright (c) 2013 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_NATIVE_WINDOW_MAC_H_
#define ATOM_BROWSER_NATIVE_WINDOW_MAC_H_

#import <Cocoa/Cocoa.h>

#include <string>
#include <vector>

#include "atom/browser/native_window.h"
#include "base/mac/scoped_nsobject.h"

@class AtomNSWindow;
@class AtomNSWindowDelegate;
@class FullSizeContentView;

namespace atom {

class NativeWindowMac : public NativeWindow {
 public:
  NativeWindowMac(brightray::InspectableWebContents* inspectable_web_contents,
                  const mate::Dictionary& options,
                  NativeWindow* parent);
  ~NativeWindowMac() override;

  // NativeWindow:
  void Close() override;
  void CloseImmediately() override;
  void Focus(bool focus) override;
  bool IsFocused() override;
  void Show() override;
  void ShowInactive() override;
  void Hide() override;
  bool IsVisible() override;
  bool IsEnabled() override;
  void SetEnabled(bool enable) override;
  void Maximize() override;
  void Unmaximize() override;
  bool IsMaximized() override;
  void Minimize() override;
  void Restore() override;
  bool IsMinimized() override;
  void SetFullScreen(bool fullscreen) override;
  bool IsFullscreen() const override;
  void SetBounds(const gfx::Rect& bounds, bool animate = false) override;
  gfx::Rect GetBounds() override;
  void SetContentSizeConstraints(
      const extensions::SizeConstraints& size_constraints) override;
  void SetResizable(bool resizable) override;
  bool IsResizable() override;
  void SetMovable(bool movable) override;
  void SetAspectRatio(double aspect_ratio, const gfx::Size& extra_size)
    override;
  void PreviewFile(const std::string& path, const std::string& display_name)
    override;
  void CloseFilePreview() override;
  bool IsMovable() override;
  void SetMinimizable(bool minimizable) override;
  bool IsMinimizable() override;
  void SetMaximizable(bool maximizable) override;
  bool IsMaximizable() override;
  void SetFullScreenable(bool fullscreenable) override;
  bool IsFullScreenable() override;
  void SetClosable(bool closable) override;
  bool IsClosable() override;
  void SetAlwaysOnTop(bool top, const std::string& level,
                      int relativeLevel, std::string* error) override;
  bool IsAlwaysOnTop() override;
  void Center() override;
  void Invalidate() override;
  void SetTitle(const std::string& title) override;
  std::string GetTitle() override;
  void FlashFrame(bool flash) override;
  void SetSkipTaskbar(bool skip) override;
  void SetSimpleFullScreen(bool simple_fullscreen) override;
  bool IsSimpleFullScreen() override;
  void SetKiosk(bool kiosk) override;
  bool IsKiosk() override;
  void SetBackgroundColor(const std::string& color_name) override;
  void SetHasShadow(bool has_shadow) override;
  bool HasShadow() override;
  void SetOpacity(const double opacity) override;
  double GetOpacity() override;
  void SetRepresentedFilename(const std::string& filename) override;
  std::string GetRepresentedFilename() override;
  void SetDocumentEdited(bool edited) override;
  bool IsDocumentEdited() override;
  void SetIgnoreMouseEvents(bool ignore, bool) override;
  void SetContentProtection(bool enable) override;
  void SetBrowserView(NativeBrowserView* browser_view) override;
  void SetParentWindow(NativeWindow* parent) override;
  gfx::NativeView GetNativeView() const override;
  gfx::NativeWindow GetNativeWindow() const override;
  gfx::AcceleratedWidget GetAcceleratedWidget() const override;
  void SetProgressBar(double progress, const ProgressState state) override;
  void SetOverlayIcon(const gfx::Image& overlay,
                      const std::string& description) override;

  void SetVisibleOnAllWorkspaces(bool visible) override;
  bool IsVisibleOnAllWorkspaces() override;

  void SetAutoHideCursor(bool auto_hide) override;

  void SelectPreviousTab() override;
  void SelectNextTab() override;
  void MergeAllWindows() override;
  void MoveTabToNewWindow() override;
  void ToggleTabBar() override;
  bool AddTabbedWindow(NativeWindow* window) override;

  void SetVibrancy(const std::string& type) override;
  void SetTouchBar(
      const std::vector<mate::PersistentDictionary>& items) override;
  void RefreshTouchBarItem(const std::string& item_id) override;
  void SetEscapeTouchBarItem(const mate::PersistentDictionary& item) override;

  gfx::Rect ContentBoundsToWindowBounds(const gfx::Rect& bounds) const;
  gfx::Rect WindowBoundsToContentBounds(const gfx::Rect& bounds) const;
  void UpdateDraggableRegions(
      const std::vector<DraggableRegion>& regions) override;

  // Set the attribute of NSWindow while work around a bug of zoom button.
  void SetStyleMask(bool on, NSUInteger flag);
  void SetCollectionBehavior(bool on, NSUInteger flag);

  enum TitleBarStyle {
    NORMAL,
    HIDDEN,
    HIDDEN_INSET,
    CUSTOM_BUTTONS_ON_HOVER,
  };
  TitleBarStyle title_bar_style() const { return title_bar_style_; }

  bool zoom_to_page_width() const { return zoom_to_page_width_; }
  bool fullscreen_window_title() const { return fullscreen_window_title_; }
  bool simple_fullscreen() const { return always_simple_fullscreen_; }
  const std::vector<DraggableRegion>& draggable_regions() const {
    return draggable_regions_;
  }

 protected:
  // Return a vector of non-draggable regions that fill a window of size
  // |width| by |height|, but leave gaps where the window should be draggable.
  std::vector<gfx::Rect> CalculateNonDraggableRegions(
      const std::vector<DraggableRegion>& regions, int width, int height);

 private:
  void InternalSetParentWindow(NativeWindow* parent, bool attach);
  void ShowWindowButton(NSWindowButton button);

  void InstallView();
  void UninstallView();

  void SetRenderWidgetHostOpaque(bool opaque);

  base::scoped_nsobject<AtomNSWindow> window_;
  base::scoped_nsobject<AtomNSWindowDelegate> window_delegate_;

  // Event monitor for scroll wheel event.
  id wheel_event_monitor_;

  // The view that will fill the whole frameless window.
  base::scoped_nsobject<FullSizeContentView> content_view_;

  NativeBrowserView* browser_view_;

  std::vector<DraggableRegion> draggable_regions_;

  bool is_kiosk_;

  bool was_fullscreen_;

  bool zoom_to_page_width_;

  bool fullscreen_window_title_;

  NSInteger attention_request_id_;  // identifier from requestUserAttention

  // The presentation options before entering kiosk mode.
  NSApplicationPresentationOptions kiosk_options_;

  // The "titleBarStyle" option.
  TitleBarStyle title_bar_style_;

  // Simple (pre-Lion) Fullscreen Settings
  bool always_simple_fullscreen_;
  bool is_simple_fullscreen_;
  bool was_maximizable_;
  bool was_movable_;
  NSRect original_frame_;
  NSUInteger simple_fullscreen_mask_;

  base::scoped_nsobject<NSColor> background_color_before_vibrancy_;
  bool transparency_before_vibrancy_;

  // The presentation options before entering simple fullscreen mode.
  NSApplicationPresentationOptions simple_fullscreen_options_;

  DISALLOW_COPY_AND_ASSIGN(NativeWindowMac);
};

}  // namespace atom

#endif  // ATOM_BROWSER_NATIVE_WINDOW_MAC_H_
