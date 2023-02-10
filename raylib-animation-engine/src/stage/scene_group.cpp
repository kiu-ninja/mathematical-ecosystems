// #include "stage/scene_group.hpp"

// void SceneGroup::_set_application_pointer(Application* application) {
//     app = application;

//     for (int i = 0; i < scenes.size(); i++) {
//         scenes[i]->_set_application_pointer(application);
//     }
// }

// SceneGroup::SceneGroup(std::vector<Scene*> _scenes) {
//     for (Scene* scene : _scenes) {
//         merge(scene);
//     }
// }

// SceneGroup* SceneGroup::merge(Scene* scene) {
//     scene->set_duration_frames(this->duration_frames);
//     scene->set_start_frames(this->start_frame);
//     merged.push_back(true);
//     return insert(scene);
// }


// SceneGroup* SceneGroup::insert(Scene* scene) {
//     scene->_set_application_pointer(this->app);

//     if (merged.size() == scenes.size()) {
//         scene->set_start_frames(start_frame);
//         merged.push_back(false);
//     }

//     scenes.push_back(scene);
//     return this;
// }

// SceneGroup* SceneGroup::set_duration_frames(const int &_duration_frames) {
//     for (int i = 0; i < scenes.size(); i++) {
//         if (merged[i])
//             scenes[i]->set_duration_frames(_duration_frames);
//     }

//     duration_frames = _duration_frames;
//     return this;
// }

// SceneGroup* SceneGroup::set_duration(const float &duration_seconds) {
//     return set_duration_frames(duration_seconds * 60);
// }

// SceneGroup* SceneGroup::set_start(const float &start_seconds) {
//     return set_start_frames(start_seconds * 60);
// }

// SceneGroup* SceneGroup::set_start_frames(const int &_start_frame) {
//     int d = _start_frame - start_frame;        
//     start_frame = _start_frame;
//     for (Scene* scene : scenes) scene->wait_frames(d);
//     return this;
// }

// Scene* SceneGroup::get_scene() {
//     for (Scene* &scene : scenes) {
//         scene = scene->get_scene();
//     }

//     _start();
//     return this;
// }

// void SceneGroup::_start() {
//     this->start();
// }

// void SceneGroup::_update_state() {
//     for (Scene* scene : scenes) {
//         scene->_update_state();
//     }
// }