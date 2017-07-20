#include "AnimationChannel.h"
#include "Base/BaseMath.h"
#include "Debug/DVAssert.h"

namespace DAVA
{
AnimationChannel::State::State()
{
    Memset(value, 0, sizeof(value));
}

const float32* AnimationChannel::State::GetChannelValue() const
{
    return value;
}

uint32 AnimationChannel::Bind(const uint8* _data)
{
    dimension = keyStride = keysCount = 0;
    keysData = nullptr;

    const uint8* dataptr = _data;
    if (_data != nullptr && *reinterpret_cast<const uint32*>(_data) == ANIMATION_CHANNEL_DATA_SIGNATURE)
    {
        dataptr += 4; //skip signature

        dimension = uint32(*dataptr);
        dataptr += 1;

        interpolation = eInterpolation(*dataptr);
        dataptr += 1;

        compression = *reinterpret_cast<const uint16*>(dataptr);
        dataptr += 2;

        keysCount = *reinterpret_cast<const uint32*>(dataptr);
        dataptr += 4;

        keysData = dataptr;

        keyStride = uint32(sizeof(float32)) * (dimension + 1);
        if (interpolation == INTERPOLATION_BEZIER)
            keyStride += uint32(sizeof(float32) * 4); //four float32 as tangents
    }

    return uint32(keysData - _data) + keysCount * keyStride;
}
#define KEY_DATA_SIZE (dimension * sizeof(float32))
#define KEY_TIME(keyIndex) (*reinterpret_cast<const float32*>(keysData + (keyIndex)*keyStride))
#define KEY_DATA(keyIndex) (reinterpret_cast<const float32*>(keysData + (keyIndex)*keyStride + sizeof(float32)))
#define KEY_META(keyIndex) (KEY_DATA(keyIndex) + KEY_DATA_SIZE) //tangents for bezier interpolation

void AnimationChannel::Reset(State* state) const
{
    state->time = KEY_TIME(0);
    state->startKey = 0;
    Memcpy(state->value, KEY_DATA(0), KEY_DATA_SIZE);
}

void AnimationChannel::Advance(float32 dTime, State* state) const
{
    state->time += dTime;

    uint32 k = state->startKey;
    for (; k < keysCount; ++k)
    {
        if (KEY_TIME(k) > state->time)
            break;

        state->startKey = k;
    }

    if (k == 0)
    {
        Memcpy(state->value, KEY_DATA(0), KEY_DATA_SIZE);
        return;
    }

    if (k == keysCount)
    {
        Memcpy(state->value, KEY_DATA(keysCount - 1), KEY_DATA_SIZE);
        return;
    }

    DVASSERT(k != 0);

    uint32 k0 = k - 1;
    float32 time0 = KEY_TIME(k0);
    float32 time1 = KEY_TIME(k);
    float32 t = (state->time - time0) / (time1 - time0);

    switch (interpolation)
    {
    case INTERPOLATION_LINEAR:
        {
            for (uint32 d = 0; d < dimension; ++d)
            {
                float32 v0 = *(KEY_DATA(k0) + d);
                float32 v1 = *(KEY_DATA(k) + d);
                state->value[d] = Lerp(v0, v1, t);
            }
        }
        break;

        case INTERPOLATION_SPHERICAL_LINEAR:
        {
            DVASSERT(dimension == 4); //should be quaternion

            Quaternion q0(KEY_DATA(k0));
            Quaternion q(KEY_DATA(k));
            q.Slerp(q0, q, t);
            q.Normalize();

            Memcpy(state->value, q.data, KEY_DATA_SIZE);
        }
        break;

        case INTERPOLATION_BEZIER:
        {
            DVASSERT(false);
        }
        break;

        default:
            break;
    }
}
#undef KEY_DATA_SIZE
#undef KEY_TIME
#undef KEY_DATA
#undef KEY_META
}
