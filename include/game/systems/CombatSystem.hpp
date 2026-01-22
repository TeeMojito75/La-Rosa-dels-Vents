#pragma once

#include <nds.h>
#include <nf_lib.h>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"

#include "game/components/TransformComponent.hpp"
#include "game/components/SpriteComponent.hpp"
#include "game/components/CollisionComponent.hpp"
#include "game/components/HealthComponent.hpp"
#include "game/components/AttackComponent.hpp"

class CombatSystem : public ecs::System {
public:
    void Init(ecs::Entity weapon) { weapon_ = weapon; hasWeapon_ = true; }

    void Update(ecs::ComponentManager& cm) {
        if (!hasWeapon_) return;

        auto& wa = cm.GetComponent<AttackComponent>(weapon_);
        if (!wa.active) return;

        auto& wt = cm.GetComponent<TransformComponent>(weapon_);
        auto& wc = cm.GetComponent<CollisionComponent>(weapon_);

        Rect hit{ wt.x, wt.y, wc.w, wc.h };

        for (auto e : mEntities) {
            auto& et = cm.GetComponent<TransformComponent>(e);
            auto& ec = cm.GetComponent<CollisionComponent>(e);
            auto& hp = cm.GetComponent<HealthComponent>(e);
            auto& sp = cm.GetComponent<SpriteComponent>(e);

            Rect er{ et.x, et.y, ec.w, ec.h };

            if (intersects(hit, er)) {
                hp.current_hp -= wa.damage;

                // Desactivar arma després d'impactar


                if (hp.current_hp <= 0) {
                    // Mou Transform perquè el SpriteSystem no el "ressusciti"
                    et.x = 256; et.y = 192;
                    NF_MoveSprite(1, sp.spriteID, 256, 192);
                }
                break;
            }
        }
    }

private:
    struct Rect { int x, y, w, h; };

    static bool intersects(const Rect& a, const Rect& b) {
        return !(a.x + a.w <= b.x || b.x + b.w <= a.x ||
                 a.y + a.h <= b.y || b.y + b.h <= a.y);
    }

private:
    ecs::Entity weapon_;
    bool hasWeapon_ = false;
};
