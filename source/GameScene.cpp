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
        renderer_.commitOAMSub(); // ✅ fuerza que el sprite se dibuje
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
    // cm_.RegisterComponent<HealthComponent>();

    // Registrar sistemas
    inputSys_  = sm_.RegisterSystem<PlayerInputSystem>();
    moveSys_   = sm_.RegisterSystem<MovementSystem>();
    spriteSys_ = sm_.RegisterSystem<SpriteSystem2D>();
    collisionSys_ = sm_.RegisterSystem<CollisionSystem>();
    collisionSys_->Init(&collisionMap_);

    // combatSys_    = sm_.RegisterSystem<CombatSystem>();

    // Tipos para signatures
    ComponentType tid = cm_.GetComponentType<TransformComponent>();
    ComponentType vid = cm_.GetComponentType<VelocityComponent>();
    ComponentType sid = cm_.GetComponentType<SpriteComponent>();
    ComponentType cid = cm_.GetComponentType<CollisionComponent>();

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

    // Signature sigCombat; sigCombat.set(hid); sigCombat.set(cid);
    // sm_.SetSignature<CombatSystem>(sigCombat);
}

void GameScene::createDemoEntities() {
    // Crear entidad jugador
    player_ = em_.createEntity();

    ComponentType tid = cm_.GetComponentType<TransformComponent>();
    ComponentType vid = cm_.GetComponentType<VelocityComponent>();
    ComponentType sid = cm_.GetComponentType<SpriteComponent>();
    ComponentType cid = cm_.GetComponentType<CollisionComponent>();

    Signature ps;
    ps.set(tid); ps.set(vid); ps.set(sid); ps.set(cid);

    em_.setSignature(player_, ps);
    sm_.EntitySignatureChanged(player_, ps);

    // Componentes iniciales (mismos valores que tu main)
    TransformComponent t{ 100, 80 };
    VelocityComponent  v{ 0, 0 };
    SpriteComponent    sp{ 0, 0, 0 };
    CollisionComponent c{14, 14, true};

    cm_.AddComponent<TransformComponent>(player_, t);
    cm_.AddComponent<VelocityComponent>(player_, v);
    cm_.AddComponent<SpriteComponent>(player_, sp);
    cm_.AddComponent<CollisionComponent>(player_, c);
}

void GameScene::run() {
    // Bucle principal según tu memoria: input → movement → (collision) → (combat) → render → VBlank/OAM :contentReference[oaicite:3]{index=3}
    while (running_) {
        inputSys_->Update(cm_);
        if (collisionSys_) collisionSys_->Update(cm_);
        moveSys_->Update(cm_);
        // if (combatSys_)    combatSys_->Update(cm_);

        spriteSys_->Update(cm_);

        // Commit gráfico
        renderer_.commitOAMSub();
    }
}
