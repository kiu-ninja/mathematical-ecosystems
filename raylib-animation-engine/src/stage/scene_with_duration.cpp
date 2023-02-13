// #include <vector>

// int TimedScene::get_duration() {
//     return duration;
// }


// void TimedScene::update() {
//     if (!should_act()) {
//         start_time--;
//     }
//     if (!should_finish()) {
//         current_frame++;
//     }
// }

// bool TimedScene::should_act() {
//     return this->start_time <= 0; 
// }
// bool TimedScene::should_finish() {
//     return this->current_frame > duration;
// }
// TimedScene*  TimedScene::duration_seconds(float time_seconds) {
//     return this->duration_frames(time_seconds * 60);
// }
// TimedScene*  TimedScene::duration_frames(int time_frames) {
//     duration = time_frames;
//     return this;
// }
// TimedScene*  TimedScene::play_with(TimedScene* other_scene) {
//     return dynamic_cast<TimedScene*>(this->duration_frames(other_scene->get_duration())->at_frames(other_scene->get_start_time()));
// }
// TimedScene*  TimedScene::play_after(TimedScene* other_scene) {
//     return dynamic_cast<TimedScene*>(this->at_frames(other_scene->get_start_time() + other_scene->get_duration()));
// }