#pragma once

#include "../../common.h"
#include <memory>
#include <deque>
#include <iostream>
#include <array>
#include <unordered_map>
// used: draw system
#include "../../sdk/datatypes/vector.h"
#include "../../sdk/datatypes/transform.h"
#include "../../sdk/datatypes/qangle.h"
#include "../../sdk/interfaces/iengineclient.h"
#include "../../sdk/interfaces/cgameentitysystem.h"
#include "../cstrike/sdk/interfaces/ienginecvar.h"
#include "../../core/hooks.h"

namespace F::LAGCOMP
{
    class c_bones
    {
    public:
        void copy();
        void set();
    protected:
        bone_data matrix_[128];
    };

    class c_lagrecord
    {
    private:
        float m_simtime_;
    public:
        bone_data matrix_[128];
        bone_data backup_matrix_[128];
    private:
        int bone_count;
        Vector_t origin_;
    public:
        void setup(C_CSPlayerPawn* player_pawn);
        void apply(C_CSPlayerPawn* player_pawn, bool extrapolation = false);
        void restore(C_CSPlayerPawn* player_pawn);
        bool record_of_bounds();
        c_lagrecord() {};
        bool valid = false;

        inline c_lagrecord(C_CSPlayerPawn* pawn) {
            this->setup(pawn);
        }
    public:
        float get_sim_time() { return m_simtime_; }
    };

    class c_animation_table
    {
    public:
        std::deque<c_lagrecord> m_lag_records_{};
        c_lagrecord* latest_record{};
        c_lagrecord* oldest_record{};
    };

    const auto lagrecords = std::make_unique<c_lagrecord>();

    class c_lagcompensation
    {
    public:
        void run();

        inline c_lagrecord* get_first_record(int handle) {
            if (m_animation.find(handle) == m_animation.end())
                return nullptr;

            auto& records = m_animation[handle];

            if (records.empty())
                return nullptr;

            auto record = &records.front();
            if (!record->valid)
                return nullptr;

            return record;
        }

        inline c_lagrecord* get_last_record(int handle) {
            if (m_animation.find(handle) == m_animation.end())
                return nullptr;

            auto& records = m_animation[handle];

            if (records.empty())
                return nullptr;

            auto record = &records.back();
            if (!record->valid)
                return nullptr;

            return record;
        }
    public:
        float get_delta_time(int tick_entity);
        std::unordered_map<int, std::deque<c_lagrecord>> m_animation{};
    public:
        int level;
    };

    const auto lagcomp = std::make_unique<c_lagcompensation>();

}
