#include "stage/scene.hpp"
#include "particle-life/state.hpp"

struct Simulation : public Scene::Scene {
    using Scene::Scene;
    int frame_count = 0;
    PLState* state;

    static const int SIM_SPACE_SIZE = 600;
    static const int GRID_CELLS = 100;
    static const int GRID_CELLS_2 = GRID_CELLS * GRID_CELLS;
    static const int CELL_SIZE = SIM_SPACE_SIZE / GRID_CELLS;
    Particle* particles[PL_N];
    int grid_cell_inds[GRID_CELLS_2];
    int cell_counts[GRID_CELLS_2];

    Simulation(PLState* new_cs) { state = new_cs; }

    Scene* begin() override { 
        for (int i = 0; i < PL_N; i++) {
            float x = randf(0, SIM_SPACE_SIZE);
            float y = randf(0, SIM_SPACE_SIZE);
            state->particles.objects[i] = new Particle(
                x,
                y,
                rand() % PL_KINDS);
                // (int)(x/SIM_SPACE_SIZE*20) % PL_KINDS);
        }
        return this;
    }
    Scene* finish() override { return this; }

    void act() override
    {
        put_particles_in_cells();
        update_velocities();
        update_positions();
    }

    void update_positions() {
        for (Drawable* p_drawable : state->particles.objects) {
            Particle* p = (Particle*)p_drawable;

            p->position.x -= (state->jeff->dimensions.x * SIM_SPACE_SIZE) * std::floor(p->position.x / (state->jeff->dimensions.x * SIM_SPACE_SIZE));
            p->position.y -= (state->jeff->dimensions.x * SIM_SPACE_SIZE) * std::floor(p->position.y / (state->jeff->dimensions.x * SIM_SPACE_SIZE));

            p->position += p->velocity;
        }
    }

    void update_velocities() {
        float min_d = state->jeff->dimensions.x * PL_KIND_RULE_START;
        float max_d = state->jeff->dimensions.x * PL_INTERACTION_RADIUS;

        for (Particle* p : particles) {
            int ind = get_particle_cell_ind(p);

            Particle* neighbors[PL_N];
            int n = get_particles_around(p, neighbors);

            Vector2 acc = {0,0};

            for (int i = 0; i < n; i++) {
                if (neighbors[i] != p) {
                    Vector2 d_vector = p->position - neighbors[i]->position;
                    float d = length(d_vector);
                    d_vector = d_vector / d;
                    if (d > max_d) {}
                    else if (d > min_d) {
                        float m = (min_d + max_d) / 2;
                        float f = 1.0f - abs(1 + min_d - 2 * d) / (1 - min_d);
                        acc -= d_vector * state->interaction_rules[p->kind][neighbors[i]->kind] * f;
                    } else {
                        float f = 1.0f - d / min_d;
                        acc += d_vector * f * 50.0f;
                    }
                }
            }

            p->velocity *= 0.98;
            p->velocity += acc * state->jeff->dimensions.x * 0.002f;

        }
    }

    int get_particles_around(Particle* p, Particle** res) {
        int count = 0;

        int x_l = (p->position.x - state->jeff->dimensions.x * PL_INTERACTION_RADIUS) / CELL_SIZE;
        int x_r = (p->position.x + state->jeff->dimensions.x * PL_INTERACTION_RADIUS) / CELL_SIZE;
        int y_l = (p->position.y - state->jeff->dimensions.x * PL_INTERACTION_RADIUS) / CELL_SIZE;
        int y_r = (p->position.y + state->jeff->dimensions.x * PL_INTERACTION_RADIUS) / CELL_SIZE;

        for (int i = std::max(x_l, 0); i <= std::min(x_r, GRID_CELLS - 1); i++) {
            for (int j = std::max(y_l, 0); j <= std::min(y_r, GRID_CELLS - 1); j++) {
                int ind = j * GRID_CELLS + i;
                for (int k = 0; k < cell_counts[ind]; k++) {
                    res[count++] = particles[grid_cell_inds[ind] + k];
                }
            }
        }

        return count;
    }

    void put_particles_in_cells() {
        for (int &x : cell_counts) x = 0;
        
        for (Drawable* p : state->particles.objects) {
            int ind = get_particle_cell_ind((Particle*)p);
            cell_counts[ind]++;
        }

        int cell_counts_copy[GRID_CELLS_2];
        grid_cell_inds[0] = 0;
        cell_counts_copy[0] = cell_counts[0];
        for (int i = 1; i < GRID_CELLS_2; i++) {
            grid_cell_inds[i] = grid_cell_inds[i - 1] + cell_counts[i - 1];
            cell_counts_copy[i] = cell_counts[i];
        }

        for (Drawable* p : state->particles.objects) {
            int ind = get_particle_cell_ind((Particle*)p);

            particles[grid_cell_inds[ind] + --cell_counts_copy[ind]] = (Particle*)p;
        }
    }

    int get_particle_cell_ind(Particle* p) {
        int x = p->position.x / (state->jeff->dimensions.x * CELL_SIZE);
        int y = p->position.y / (state->jeff->dimensions.x * CELL_SIZE);
        int i = std::min(std::max(x, 0), GRID_CELLS - 1);
        int j = std::min(std::max(y, 0), GRID_CELLS - 1);

        return j * GRID_CELLS + i;
    }
};