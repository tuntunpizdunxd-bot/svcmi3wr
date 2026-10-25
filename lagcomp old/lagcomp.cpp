#include "lagcomp.h"

namespace F::LAGCOMP
{
    void c_lagcompensation::run()
    {

        const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

        if (!SDK::LocalPawn)
            return;

        if (!I::Engine->IsConnected() && !I::Engine->IsInGame())
            return;

        static auto sv_maxunlag = I::Cvar->Find(FNV1A::HashConst("sv_maxunlag"));
        const int max_ticks = TIME_TO_TICKS(sv_maxunlag->get_float());

        for (int i = 1; i <= 64; i++)
        {
            auto entity = I::GameResourceService->pGameEntitySystem->Get< C_BaseEntity >(i);
            if (entity == nullptr) {
                continue;
            }

            SchemaClassInfoData_t* pClassInfo = nullptr;
            entity->GetSchemaClassInfo(&pClassInfo);

            if (pClassInfo == nullptr)
                continue;

            const FNV1A_t uHashedName = FNV1A::Hash(pClassInfo->szName);

            if (uHashedName != FNV1A::HashConst(CS_XOR("CCSPlayerController"))) {
                continue;
            }

            CCSPlayerController* player_controller = reinterpret_cast<CCSPlayerController*>(entity);
            if (player_controller == nullptr)
                continue;

            if (!player_controller || player_controller == SDK::LocalController)
                continue;

            auto handle = player_controller->GetRefEHandle().GetInt();


            if (!player_controller->IsPawnAlive()) {
                auto player_iterator = m_animation.find(handle);
                if (player_iterator != m_animation.end())
                    m_animation.erase(player_iterator);

                continue;
            }

            C_CSPlayerPawn* player_pawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(player_controller->GetPawnHandle());
            if (!player_pawn) {
                auto player_iterator = m_animation.find(handle);
                if (player_iterator != m_animation.end())
                    m_animation.erase(player_iterator);

                continue;
            }

            if (m_animation.find(handle) == m_animation.end()) {
                m_animation.insert_or_assign(handle, std::deque<c_lagrecord>{});

                continue;
            }

            auto& records = m_animation[handle];

            if (records.size() != max_ticks) {
                records.clear();
                records.resize(max_ticks);
            }

            auto& record = records.emplace_front(player_pawn);
            if (records.size() > max_ticks - 1)
                records.erase(records.end() - 1);
        }
    }


    struct data_info
    {
        std::uint64_t tick;
        float time;
        char pad[0x24];
    };

    float c_lagcompensation::get_delta_time(int tick_entity)
    {
        static auto sv_maxunlag = I::Cvar->Find(FNV1A::HashConst("sv_maxunlag"));

        int tick_count = cheat->tickbase;
        float maxunlag_val = std::min(sv_maxunlag->get_float(), 0.2f);

        float mod_value = fmodf(maxunlag_val, 0.015625);
        float unlag_difference = maxunlag_val - mod_value;
        float mod_ticks = mod_value * 64.0;
        int unlag_ticks = (int)(float)((float)(unlag_difference * 64.0) + 0.5);

        data_info info{};
        info.tick = tick_entity;
        info.time = tick_entity * 0.015625f;

        int v20{};
        float v19{}, v21{};

        if (mod_ticks < (float)(1.0 - 0.0099999998))
        {
            v20 = tick_count - unlag_ticks;
            if (mod_ticks > 0.0099999998)
            {
                v19 = -mod_ticks;
                v21 = v19;
                if (v19 < 0.0f)
                    v19 = v19 + 1.0f;
            }
            else
            {
                v19 = 0.0f;
                v21 = 0.0;
            }
        }
        else // high ping 100+
        {
            v19 = 0.0f;
            v20 = tick_count - (unlag_ticks + 1);
            v21 = 0.0;
        }

        auto v22 = v20 - 1;

        data_info v40{};

        v40.time = v19;
        if (v21 >= 0.0)
            v22 = v20;
        v40.tick = v22;
        if (v19 < (float)(1.0 - 0.0099999998))
        {
            if (v19 <= 0.0099999998)
            {
                v40.time = 0.0;
            }
        }
        else
        {
            v40.tick = v22 + 1;
            v40.time = 0.0;
        }

        auto tick = v40.tick;
        auto v24 = (float)info.tick;
        auto v38 = v40;
        int new_tick{};
        data_info v39{};

        if ((float)((float)(v24 + info.time) * 0.015625) <= 0.0)
        {

            auto cmd_tick_count = tick_entity;
            v40.tick = cmd_tick_count;
            v40.time = 0.0;
            new_tick = v40.tick;
            tick = v38.tick;
            v39 = v40;
        }
        else
        {
            new_tick = info.tick;
            v39 = info;
        }

        int v32{};
        float v30{};
        int v31{};
        int v33{};

        if (tick <= new_tick && (tick < new_tick || v39.time > v38.time))
        {
            v33 = new_tick - tick;
            v30 = v39.time - v38.time;
            if ((float)(v39.time - v38.time) < 0.0)
                v30 = v30 + 1.0;
            v32 = v33 - 1;
            if ((float)(v39.time - v38.time) >= 0.0)
                v32 = v33;
            if (v30 >= (float)(1.0 - 0.0099999998))
            {
                ++v32;
                goto LABEL_48;
            }
        LABEL_47:
            if (v30 > 0.0099999998)
                goto LABEL_49;
            goto LABEL_48;
        }
        v30 = v38.time - v39.time;
        v31 = tick - new_tick;
        if ((float)(v38.time - v39.time) < 0.0)
            v30 = v30 + 1.0;
        v32 = v31 - 1;
        if ((float)(v38.time - v39.time) >= 0.0)
            v32 = v31;
        if (v30 < (float)(1.0 - 0.0099999998))
            goto LABEL_47;
        ++v32;
    LABEL_48:
        v30 = 0.0;
    LABEL_49:
        auto delta_time = (float)((float)v32 + v30) * 0.015625;

        return delta_time;
    }

    void c_lagrecord::setup(C_CSPlayerPawn* player_pawn)
    {
        if (!player_pawn || !player_pawn->GetGameSceneNode()->GetSkeletonInstance()->get_bone_count())
            return;

        auto game_scene_node = player_pawn->GetGameSceneNode();
        if (!game_scene_node)
            return;

        auto skeleton = game_scene_node->GetSkeletonInstance();
        if (!skeleton)
            return;
        auto model_state = &skeleton->GetModel();

        CStrongHandle<CModel> model = model_state->m_hModel();
        auto model_skelet = &model->m_modelSkeleton();

        if (!model_skelet)
            return;

        m_simtime_ = player_pawn->m_flSimulationTime();
        origin_ = player_pawn->GetGameSceneNode()->GetAbsOrigin();
        bone_count = model->GetHitboxesNum();
        memcpy(matrix_, model_state->GetHitboxData(), bone_count * sizeof(bone_data));

        valid = true;
    }

    void c_lagrecord::apply(C_CSPlayerPawn* player_pawn, bool extrapolation)
    {
        if (!player_pawn || !player_pawn->GetGameSceneNode()->GetSkeletonInstance()->get_bone_count())
            return;

        auto game_scene_node = player_pawn->GetGameSceneNode();
        if (!game_scene_node)
            return;

        auto skeleton = game_scene_node->GetSkeletonInstance();
        if (!skeleton)
            return;
        auto model_state = &skeleton->GetModel();

        CStrongHandle<CModel> model = model_state->m_hModel();
        auto model_skelet = &model->m_modelSkeleton();

        if (!model_skelet)
            return;

        memcpy(backup_matrix_, player_pawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData(), model->GetHitboxesNum() * sizeof(bone_data));

        memcpy(player_pawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData(), matrix_, model->GetHitboxesNum() * sizeof(bone_data));
        //if (extrapolation)
        //{
        //    for (int i{}; i < 128; i++) {
        //        player_pawn->get_skeleton()->m_modelState().get_bone_data()[i].pos += ((0.017f * player_pawn->velocity()) * ((Interfaces::m_network_game_service->get_network_game_client()->get_net_channel2()->get_network_latency() * 64.f) + 0.5f));
        //    }
        //}
    }

    void c_lagrecord::restore(C_CSPlayerPawn* player_pawn)
    {
        auto game_scene_node = player_pawn->GetGameSceneNode();
        if (!game_scene_node)
            return;

        auto skeleton = game_scene_node->GetSkeletonInstance();
        if (!skeleton)
            return;
        auto model_state = &skeleton->GetModel();

        CStrongHandle<CModel> model = model_state->m_hModel();
        auto model_skelet = &model->m_modelSkeleton();

        if (!model_skelet)
            return;

        memcpy(player_pawn->GetGameSceneNode()->GetSkeletonInstance()->GetModel().GetHitboxData(), backup_matrix_, model->GetHitboxesNum() * sizeof(bone_data));
    }

    bool c_lagrecord::record_of_bounds()
    {
        static auto sv_maxunlag = I::Cvar->Find(FNV1A::HashConst("sv_maxunlag"));
        float max_unlag = std::min(sv_maxunlag->get_float(), 0.2f);

        const auto nci = I::Engine->GetNetChannelInfo(0);
        if (!nci)
            return std::numeric_limits<float>::max();

        float level[3];
        level[0] = max_unlag / 3.f;
        level[1] = max_unlag / 2.f;
        level[2] = max_unlag;

        const auto latency = nci->get_latency(flow::FLOW_OUTGOING) * 0.5f + nci->get_latency(FLOW_INCOMING);

        float delta_time = lagcomp->get_delta_time(get_sim_time() * 64.f) + latency;

        return std::fabsf(delta_time) < level[2];
    }
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////


#include "lagcomp.h"

namespace F::LAGCOMP
{
    void CLagCompensation::Main() {
        // TODO: make this calc the server curtime.
        // on server it is: TIME_TO_TICKS( Interfaces::GlobalVars->m_flCurTime - sv_maxunlag->value.fl )
        const std::lock_guard<std::mutex> guard{ g_cachedEntitiesMutex };

        if (!SDK::LocalPawn)
            return;

        if (!I::Engine->IsConnected() && !I::Engine->IsInGame())
            return;

        static auto sv_maxunlag = I::Cvar->Find(FNV1A::HashConst("sv_maxunlag"));
        const int deadTicks{ I::GlobalVars->nTickCount + 1 + TIME_TO_TICKS(SDK::LocalController->GetPing() / 1000.f) - TIME_TO_TICKS(sv_maxunlag->get_float()) };

        const auto local{ SDK::LocalPawn };

        static bool removalSwitch{ };
        removalSwitch = !removalSwitch;

        for (const auto& it : g_cachedEntities) {
            const auto ent{ I::GameResourceService->pGameEntitySystem->Get<C_BaseEntity>(it.m_handle) };
            if (!ent
                || ent == local)
                continue;

            SchemaClassInfoData_t* classInfo{ };
            ent->GetSchemaClassInfo(&classInfo);
            if (!classInfo)
                continue;

            const auto hashedName{ FNV1A::Hash(classInfo->szName) };

            if (hashedName == FNV1A::HashConst("CCSPlayerController")) {
                auto playerController{ static_cast<CCSPlayerController*>(ent) };

                auto pawn{ I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(playerController->GetPawnHandle()) };
                if (!pawn)
                    continue;

                CachedEntity_t& entry{ g_cachedEntities[pawn->GetRefEHandle().nIndex] };


                if (entry.m_pPawn != pawn)
                    entry.Reset(pawn);

                entry.m_bRemovalSwitch = removalSwitch;

                if (entry.Animations.m_vecLagRecords.size()) {
                    for (auto it{ entry.Animations.m_vecLagRecords.begin() }; it != entry.Animations.m_vecLagRecords.end(); it = std::next(it)) {
                        const auto ticks{ it->m_nPlayerTickCount };
                        if (ticks >= deadTicks)
                            break;

                        it = entry.Animations.m_vecLagRecords.erase(it);
                        if (!entry.Animations.m_vecLagRecords.size())
                            break;
                    }
                }

                if (entry.Animations.m_vecLagRecords.empty()
                    || entry.m_pPawn->m_flSimulationTime() != entry.Animations.m_vecLagRecords.back().m_flSimulationTime)
                    AddRecord(entry);
            }
        }

        if (g_cachedEntities.empty())
            return;

        for (auto it{ g_cachedEntities.begin() };
            it != g_cachedEntities.end(); ) {
            if (it->m_bRemovalSwitch == removalSwitch) {
                it = std::next(it);
                continue;
            }

            it = g_cachedEntities.erase(it);
        }
    }











}