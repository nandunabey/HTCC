// Copyright (c) 2022-present Frederick Emmott
// SPDX-License-Identifier: MIT
#pragma once

#include <openxr/openxr.h>

#include <tuple>

#include "InputSource.h"
#include "OpenXRNext.h"

namespace HandTrackedCockpitClicking {

class HandTrackingSource final : public InputSource {
 public:
  HandTrackingSource(
    const std::shared_ptr<OpenXRNext>& next,
    XrInstance instance,
    XrSession session,
    XrSpace viewSpace,
    XrSpace localSpace);
  ~HandTrackingSource();

  std::tuple<InputState, InputState> Update(PointerMode, const FrameInfo&)
    override;

  void KeepAlive(XrHandEXT, const FrameInfo&);

 private:
  std::shared_ptr<OpenXRNext> mOpenXR;
  XrInstance mInstance {};
  XrSession mSession {};
  XrSpace mViewSpace {};
  XrSpace mLocalSpace {};

  struct Hand {
    XrHandEXT mHand;
    InputState mState {mHand};
    XrHandTrackerEXT mTracker {};
    std::optional<XrResult> mTrackerError;
    bool mSleeping {true};
    XrTime mLastKeepAliveAt {};
    XrTime mWakeConditionsSince {};
    XrTime mHibernateGestureSince {};

    ActionState mRawActions {};
    XrTime mRawActionsSince {};

    // --- Added for grab‑and‑move scroll ---
    float mLastScrollY = 0.0f;
    bool mScrolling = false;

    // --- Joint-based pinch detection (no XR_FB_hand_tracking_aim) ---
    // 0 = none, 1 = index, 2 = middle, 3 = ring, 4 = little
    uint8_t mJointPinchFinger {0};
    XrHandTrackingAimFlagsFB mJointPinchStatus {0};
    bool mWasTracked {false};
    XrTime mIgnoreActionsUntil {};

    // --- Twist-to-turn ---
    std::optional<XrVector3f> mTwistReferenceLateral;
  };

  bool mHibernating {false};
  XrTime mLastHibernationChangeAt {};

  Hand mLeftHand {XR_HAND_LEFT_EXT};
  Hand mRightHand {XR_HAND_RIGHT_EXT};

  void InitHandTracker(Hand* hand);
  static void UpdateJointPinch(
    Hand* hand,
    const XrHandJointLocationEXT* joints);
  static std::optional<float> UpdateTwist(
    Hand* hand,
    const XrHandJointLocationEXT* joints,
    bool pinching);
  void UpdateHand(const FrameInfo&, Hand* hand);
  static void UpdatePinchAndDragScrolling(Hand* hand, InputState& state);
  std::tuple<XrPosef, XrVector2f> RaycastPose(
    const FrameInfo&,
    const XrPosef& pose);

  enum class BeepEvent {
    Wake,
    Sleep,
    HibernateWake,
    HibernateSleep,
  };
  void PlayBeeps(BeepEvent) const;
};

}// namespace HandTrackedCockpitClicking
