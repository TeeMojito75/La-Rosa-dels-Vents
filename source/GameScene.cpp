#include "game/inits/GameScene.hpp"

using namespace ecs;

GameScene::GameScene(BackgroundRenderer& renderer)
: renderer_(renderer) {}

bool GameScene::init() {
    if (!renderer_.init()) return false;
    renderer_.loadResources();
    renderer_.setupScene();

    initECS();

    // Cargar CSV (solo una vez)
    if (!collisionMap_.loadCSV("Backgrounds/Bottom/Bottom_collision16_final.csv")) {
        printf("ERROR loading collision CSV\n");
        renderer_.commitOAMSub(); 
        while (1) swiWaitForVBlank();
    }

    collisionSys_->Init(&collisionMap_);

    createDemoEntities();
    return true;
}


void GameScene::initECS() {

    // Registrar componentes
    cm_.RegisterComponent<TransformComponent>();
    cm_.RegisterComponent<VelocityComponent>();
    cm_.RegisterComponent<SpriteComponent>();
    cm_.RegisterComponent<CollisionComponent>();
    cm_.RegisterComponent<AttackComponent>();
    cm_.RegisterComponent<HealthComponent>();


    // Registrar sistemas
    inputSys_  = sm_.RegisterSystem<PlayerInputSystem>();
    moveSys_   = sm_.RegisterSystem<MovementSystem>();
    spriteSys_ = sm_.RegisterSystem<SpriteSystem2D>();
    collisionSys_ = sm_.RegisterSystem<CollisionSystem>();
    collisionSys_->Init(&collisionMap_);
    combatSys_ = sm_.RegisterSystem<CombatSystem>();

    // Tipos para signatures
    ComponentType tid = cm_.GetComponentType<TransformComponent>();
    ComponentType vid = cm_.GetComponentType<VelocityComponent>();
    ComponentType sid = cm_.GetComponentType<SpriteComponent>();
    ComponentType cid = cm_.GetComponentType<CollisionComponent>();
    ComponentType hid = cm_.GetComponentType<HealthComponent>();
    ComponentType aid = cm_.GetComponentType<AttackComponent>();

    // Signature PlayerInputSystem: necesita Velocity (y en tu código también Sprite para facing/moving)
    Signature sigInput;
    sigInput.set(vid);
    sigInput.set(sid);
    sm_.SetSignature<PlayerInputSystem>(sigInput);

    // Signature MovementSystem: Transform + Velocity
    Signature sigMove;
    sigMove.set(tid);
    sigMove.set(vid);
    sm_.SetSignature<MovementSystem>(sigMove);

    // Signature SpriteSystem2D: Transform + Sprite
    Signature sigSprite;
    sigSprite.set(tid);
    sigSprite.set(sid);
    sm_.SetSignature<SpriteSystem2D>(sigSprite);

    // Signature Collisions: Transform + Velocity + Collision
    Signature sigCollision; 
    sigCollision.set(tid); 
    sigCollision.set(cid); 
    sigCollision.set(vid);
    sm_.SetSignature<CollisionSystem>(sigCollision);

    Signature sigCombat;
    sigCombat.set(tid);
    sigCombat.set(cid);
    sigCombat.set(hid);
    sigCombat.set(sid);
    sigCombat.set(aid);
    sm_.SetSignature<CombatSystem>(sigCombat);

}

void GameScene::createDemoEntities() {
    // Crear entidad jugador
    player_ = em_.createEntity();

    ComponentType tid = cm_.GetComponentType<TransformComponent>();
    ComponentType vid = cm_.GetComponentType<VelocityComponent>();
    ComponentType sid = cm_.GetComponentType<SpriteComponent>();
    ComponentType cid = cm_.GetComponentType<CollisionComponent>();
    ComponentType hid = cm_.GetComponentType<HealthComponent>();
    ComponentType aid = cm_.GetComponentType<AttackComponent>();

    Signature ps;
    ps.set(tid); ps.set(vid); ps.set(sid); ps.set(cid); ps.set(hid); ps.set(aid);

    em_.setSignature(player_, ps);
    sm_.EntitySignatureChanged(player_, ps);

    // Componentes iniciales (mismos valores que tu main)
    TransformComponent t{ 100, 80 };
    VelocityComponent  v{ 0, 0 };
    SpriteComponent    sp{ 0, 1, 0 };
    CollisionComponent c{14, 14, true};
    HealthComponent h{3, 3};
    AttackComponent at{1, 10, true, true};

    cm_.AddComponent<TransformComponent>(player_, t);
    cm_.AddComponent<VelocityComponent>(player_, v);
    cm_.AddComponent<SpriteComponent>(player_, sp);
    cm_.AddComponent<CollisionComponent>(player_, c);
    cm_.AddComponent<HealthComponent>(player_, h);
    cm_.AddComponent<AttackComponent>(player_, at);

    // ===== Enemigo (mínimo: animación + posición) =====
    // Enemy entity
    ecs::Entity enemy = em_.createEntity();

    Signature es;
    es.set(tid);
    es.set(sid);
    es.set(hid);
    es.set(cid);
    es.set(aid);

    em_.setSignature(enemy, es);
    sm_.EntitySignatureChanged(enemy, es);

    // Posición del enemigo
    TransformComponent te{ 200, 80 };

    // SpriteComponent MINIMAL (SIN animación)
    SpriteComponent se;
    se.spriteID = 1;   // enemySpriteId_
    se.gfxID    = 1;   // enemyGfxId_
    se.palID    = 1;   // enemyPalId_

    // Fuerza estado neutro
    se.facing = Facing::Down;
    se.moving = false;
    se.animStep = 0;
    se.animTick = 0;
    HealthComponent he;
    he.hp = 3;
    he.current_hp = 3;
    CollisionComponent ec{14, 14, true};

    cm_.AddComponent<CollisionComponent>(enemy, ec);
    cm_.AddComponent<HealthComponent>(enemy, he);
    cm_.AddComponent<TransformComponent>(enemy, te);
    cm_.AddComponent<SpriteComponent>(enemy, se);


    // ===== Arma =====
    // Arma entity
    weapon_ = em_.createEntity();

    Signature ws;
    ws.set(tid); ws.set(sid); ws.set(cid); ws.set(aid);

    em_.setSignature(weapon_, ws);
    sm_.EntitySignatureChanged(weapon_, ws);

    // Transform amagada
    TransformComponent wt{256, 192};

    // Sprite arma (spriteId=2, gfxId=2, palId=2)
    SpriteComponent wsp{};
    wsp.spriteID = 2;
    wsp.gfxID    = 2;
    wsp.palID    = 2;
    wsp.facing   = Facing::Down;
    wsp.moving   = false;

    // Hitbox arma 16x16, no solid
    CollisionComponent wc{16, 16, false};

    // Attack data
    AttackComponent wa{};
    wa.damage = 1;
    wa.ttlFrames = 0;
    wa.active = false;
    wa.fromPlayer = true;

    cm_.AddComponent<TransformComponent>(weapon_, wt);
    cm_.AddComponent<SpriteComponent>(weapon_, wsp);
    cm_.AddComponent<CollisionComponent>(weapon_, wc);
    cm_.AddComponent<AttackComponent>(weapon_, wa);

    // Ara que tenim les entitats inicialitzem el sistema de combat
    combatSys_->Init(weapon_);
}

void GameScene::spawnPlayerAttack() {
    auto& wa  = cm_.GetComponent<AttackComponent>(weapon_);
    if (wa.active) return;

    auto& pt  = cm_.GetComponent<TransformComponent>(player_);
    auto& ps  = cm_.GetComponent<SpriteComponent>(player_);

    auto& wt  = cm_.GetComponent<TransformComponent>(weapon_);
    auto& wsp = cm_.GetComponent<SpriteComponent>(weapon_);

    wa.active = true;
    wa.ttlFrames = 8;

    // Copia IDs correctos del arma (por si acaso)
    wsp.spriteID = 2;
    wsp.gfxID    = 2;
    wsp.palID    = 2;

    // Posicionar delante del jugador
    wt = pt;
    constexpr int off = 16;

    int frame = 0;
    switch (ps.facing) {
        case Facing::Up:    wt.y -= off; frame = 0; break;
        case Facing::Down:  wt.y += off; frame = 1; break;
        case Facing::Right: wt.x += off; frame = 2; break;
        case Facing::Left:  wt.x -= off; frame = 3; break;
    }

    // Seleccionar frame del spritesheet (0..3)
    NF_SpriteFrame((int)Screen::Sub, wsp.spriteID, frame);

    // Mover sprite (si tu SpriteSystem no lo hace aún)
    NF_MoveSprite((int)Screen::Sub, wsp.spriteID, wt.x, wt.y);
}

void GameScene::updateWeaponTTL() {
    auto& wa = cm_.GetComponent<AttackComponent>(weapon_);
    if (!wa.active) return;

    if (--wa.ttlFrames <= 0) {
        wa.active = false;
        auto& wt = cm_.GetComponent<TransformComponent>(weapon_);
        wt.x = 256; wt.y = 192;

        auto& wsp = cm_.GetComponent<SpriteComponent>(weapon_);
        NF_MoveSprite((int)Screen::Sub, wsp.spriteID, 256, 192);
    }
}

void GameScene::run() {
    // Bucle principal según tu memoria: input → movement → (collision) → (combat) → render → VBlank/OAM :contentReference[oaicite:3]{index=3}
    while (running_) {
        scanKeys();
        uint16 down = keysDown();
        uint16 held = keysHeld();

        inputSys_->Update(cm_, held);

        if (down & KEY_A) spawnPlayerAttack();
        updateWeaponTTL();
        
        if (collisionSys_) collisionSys_->Update(cm_);
        moveSys_->Update(cm_);

        if (combatSys_) combatSys_->Update(cm_);

        spriteSys_->Update(cm_);
        renderer_.commitOAMSub();
    }


}
