#pragma once

#include "../Actor/LocalPlayer.hpp"
#include <cstdint>
#include "../../../Utils/Memory/Memory.hpp"

class ClientInstance {
public:
	uintptr_t** VTable;

	LocalPlayer* getLocalPlayer() {
	return Memory::CallVFunc<27, LocalPlayer*>(this);
	}

    void grabMouse() {
        return Memory::CallVFunc<304, void>(this);
    }

    void releaseMouse() {
        return Memory::CallVFunc<305, void>(this);
    }

    void refocusMouse() {
        return Memory::CallVFunc<306, void>(this);
    }
};
