/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/**
 * \file nanogui/animatorStep.h
 *
 * \brief Defines the Animator for step by step animation.
 */

#pragma once

#include <chrono>
#include <vector>

#include <nanogui/widget.h>
#include <nanogui/animator.h>
#include <nanogui/types.h>

NAMESPACE_BEGIN(nanogui)

template <typename T>
class NANOGUI_EXPORT AnimatorStep : public IAnimatorBase {
public:

    AnimatorStep();
    virtual ~AnimatorStep();

    /// Sets the start value for animation.
    void setStartValue(T value);
    /// Returns the start value.
    T getStartValue();

    /// Sets the end value for animation.
    void setEndValue(T value);
    /// Returns the end value.
    T getEndValue();

    /// Sets the duration of the animation
    void setDuration(types::Duration_t value);
    /// Returns the duration of the animation
    types::Duration_t getDuration();
    
    /// Sets the duration of the animation
    void setDuration(unsigned int value);

    /// Sets the curve type for Evaluator
    void setCurveType(types::EasingCurveType type);
    /// Returns the curve type
    types::EasingCurveType getCurveType();

    /// Evaluate animation use current time.
    void animate();
    /// Initializes the Animator and Evaluator before starting the animation calculation.
    void start();

    /// Аdds the next step in the animation.
    void addStep(EvaluatorParams<T> step);

    /// Functor for get value for widget.
    std::function<T()> mGetterFunc;
    /// Functor for set value in widget.
    std::function<void(T)> mSetterFunc;

private:

    Animator<T> mAnimator;
    std::vector<EvaluatorParams<T>> mStepParamsList;
    unsigned int mNextIndex;
};

template <typename T>
AnimatorStep<T>::AnimatorStep() {
    mNextIndex = 1;
}

template <typename T>
AnimatorStep<T>::~AnimatorStep() {
}

template <typename T>
void AnimatorStep<T>::start() {
    mNextIndex = 1;

    if (!mStepParamsList.empty()) {
        mAnimator.setStartValue(mStepParamsList[0].startValue);
        mAnimator.setEndValue(mStepParamsList[0].endValue);
        mAnimator.setDuration(mStepParamsList[0].duration);
        mAnimator.setCurveType(mStepParamsList[0].curve);
    }

    mAnimator.mGetterFunc = mGetterFunc;
    mAnimator.mSetterFunc = mSetterFunc;
    mAnimator.start();
}

template <typename T>
void AnimatorStep<T>::setStartValue(T value) {
    mAnimator.setStartValue(value);
}

template <typename T>
T AnimatorStep<T>::getStartValue() {
    return mAnimator.getStartValue();
}

template <typename T>
void AnimatorStep<T>::setEndValue(T value) {
    mAnimator.setEndValue(value);
}

template <typename T>
T AnimatorStep<T>::getEndValue() {
    return mAnimator.getStartValue();
}

template <typename T>
void AnimatorStep<T>::animate() {
    if (mAnimator.isFinished() && !mStepParamsList.empty() && (mNextIndex + 1) <= mStepParamsList.size()) {
        mAnimator.setStartValue(mStepParamsList[mNextIndex].startValue);
        mAnimator.setEndValue(mStepParamsList[mNextIndex].endValue);
        mAnimator.setDuration(mStepParamsList[mNextIndex].duration);
        mAnimator.setCurveType(mStepParamsList[mNextIndex].curve);

        mAnimator.start();

        ++mNextIndex;
    }

    mAnimator.animate();
}

template <typename T>
void AnimatorStep<T>::addStep(EvaluatorParams<T> step) {
    mStepParamsList.push_back(step);
}

template <typename T>
void AnimatorStep<T>::setDuration(types::Duration_t value) {
    mAnimator.setDuration(value);
}

template <typename T>
types::Duration_t AnimatorStep<T>::getDuration() {
    return mAnimator.getDuration();
}

template <typename T>
void AnimatorStep<T>::setDuration(unsigned int value) {
  std::chrono::milliseconds m(value);
  mAnimator.setDuration(std::chrono::duration_cast<types::Duration_t>(m));
}

template <typename T>
void AnimatorStep<T>::setCurveType(types::EasingCurveType type) {
    mAnimator.setCurveType(type);
}

template <typename T>
types::EasingCurveType AnimatorStep<T>::getCurveType() {
    return mAnimator.getCurveType();
}

NAMESPACE_END(nanogui)
