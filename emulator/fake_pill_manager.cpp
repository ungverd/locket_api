#include "fake_pill_manager.h"

IdOnlyState FakePillManager::ReadPill() {
    return state;
}

void FakePillManager::WritePill(const IdOnlyState &state) {
    this->state = state;
}
