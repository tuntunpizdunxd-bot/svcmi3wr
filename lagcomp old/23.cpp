#include "lagcomp.h"

namespace F::LAGCOMP
{
    void c_lagcompensation::Create(C_CSPlayerPawn* target)
    {
        if (!target->GetGameSceneNode() || !target->GetGameSceneNode()->GetSkeletonInstance()->m_pBoneCache)
            return;

        vecOrigin = target->GetGameSceneNode()->m_vecOrigin();
        vecAbsOrigin = target->GetGameSceneNode()->GetAbsOrigin();
        angRotation = target->GetGameSceneNode()->GetAngleRotation();
        vecAbsRotation = target->GetGameSceneNode()->GetAbsAngleRotation();
        vecEyeAngles = target->GetEyePosition();


        CRT::MemoryCopy(target->GetGameSceneNode()->GetSkeletonInstance()->m_pBoneCache, &mBoneArray,
            sizeof(Matrix4x2_t) * 128);
    }

    void c_lagcompensation::Apply(C_CSPlayerPawn* target)
    {
        if (!target->GetGameSceneNode() || !target->GetGameSceneNode()->GetSkeletonInstance()->m_pBoneCache)
            return;

        vecOrigin = target->GetGameSceneNode()->m_vecOrigin();
        vecAbsOrigin = target->GetGameSceneNode()->GetAbsOrigin();
        angRotation = target->GetGameSceneNode()->GetAngleRotation();
        vecAbsRotation = target->GetGameSceneNode()->GetAbsAngleRotation();
        vecEyeAngles = target->GetEyePosition();

        CRT::MemoryCopy(&BackupBoneArray, target->GetGameSceneNode()->GetSkeletonInstance()->m_pBoneCache,
            sizeof(Matrix4x2_t) * 128);

        target->GetGameSceneNode()->m_vecOrigin() = vecOrigin;
        target->GetGameSceneNode()->GetAbsOrigin() = vecAbsOrigin;
        target->GetGameSceneNode()->GetAngleRotation() = angRotation;
        target->GetGameSceneNode()->GetAbsAngleRotation() = vecAbsRotation;

        CRT::MemoryCopy(target->GetGameSceneNode()->GetSkeletonInstance()->m_pBoneCache, &mBoneArray,
            sizeof(Matrix4x2_t) * 128);
    }

    void c_lagcompensation::Restore(C_CSPlayerPawn* target)
    {
        if (!target->GetGameSceneNode() || !target->GetGameSceneNode()->GetSkeletonInstance()->m_pBoneCache)
            return;

        target->GetGameSceneNode()->m_vecOrigin() = vecBackupOrigin;
        target->GetGameSceneNode()->GetAbsOrigin() = vecBackupAbsOrigin;
        target->GetGameSceneNode()->GetAngleRotation() = angBackupAngRotation;
        target->GetGameSceneNode()->GetAbsAngleRotation() = angBackupAngAbsRotation;
        target->GetEyePosition() = vecEyeAngles;
        CRT::MemoryCopy(target->GetGameSceneNode()->GetSkeletonInstance()->m_pBoneCache, &BackupBoneArray,
            sizeof(Matrix4x2_t) * 128);
    }
}