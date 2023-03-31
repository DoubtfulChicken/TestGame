#include <splashkit.h>
#include <string>
#include <cmath>

#define RADIUS 50
#define SMALL_RADIUS 10

using namespace std;

bool circles_collide(double x1, double y1, double r1, double x2, double y2, double r2)
{
    double distance_squared = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    double radius_sum = r1 + r2;
    return distance_squared <= radius_sum * radius_sum;
}

// Add this function to calculate the new speeds after collision
void resolve_collision(double x1, double y1, double &vx1, double &vy1, double x2, double y2, double &vx2, double &vy2, double r1, double r2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double distance = sqrt (dx * dx + dy * dy);
    double unit_contact_x = dx / distance;
    double unit_contact_y = dy / distance;

    double relative_velocity_x = vx1 - vx2;
    double relative_velocity_y = vy1 - vy2;

    double impulse = (relative_velocity_x * unit_contact_x + relative_velocity_y * unit_contact_y) / (1 / r1 + 1 / r2);

    vx1 -= impulse * unit_contact_x / r1;
    vy1 -= impulse * unit_contact_y / r1;

    vx2 += impulse * unit_contact_x / r2;
    vy2 += impulse * unit_contact_y / r2;

}

int main()
{
    double x;
    double y;
    double small_x1 = 100;
    double small_y1 = 100;
    double small_x_speed1 = 8;
    double small_y_speed1 = 6;
    double small_x2 = 400;
    double small_y2 = 200;
    double small_x_speed2 = 7;
    double small_y_speed2 = 7;
    color circle_colour = COLOR_GREEN;

    open_window("Circle Moving", 800, 600);

    x = screen_width() / 2;
    y = screen_height() - RADIUS -20;

    // Countdown loop
    for (int countdown = 10; countdown >= 1; --countdown)
    {
        clear_screen(COLOR_WHITE);

        // draw the big circle
        fill_circle(circle_colour, x, y, RADIUS);

        // draw the first small circle
        fill_circle(COLOR_RED, small_x1, small_y1, SMALL_RADIUS);

        // draw the second small circle
        fill_circle(COLOR_BLUE, small_x2, small_y2, SMALL_RADIUS);

        draw_text(to_string(countdown), COLOR_BLACK, "arial", 500, screen_width() / 2, screen_height() /2 + 30);
        draw_text("Try to dodge the small balls!", COLOR_BLACK, "arial", 24, screen_width() / 2 - 110, screen_height() / 2 + 60);
        draw_text("Use the arrow keys to move the green ball up, down, left and right!", COLOR_BLACK, "arial", 24, screen_width() / 2 - 280, screen_height() / 2 + 80);
        draw_text("Press 'C' to change colour! Idk why, why just can!", COLOR_BLACK, "arial", 24, screen_width() / 2 - 200, screen_height() / 2 + 100);
        refresh_screen(60);
        delay(1000);
    }

    sound_effect collision_sound = load_sound_effect("collision", "ram.mp3");

    // Add flags for the collision status
    bool small_ball1_in_collision = false;
    bool small_ball2_in_collision = false;

    double big_x_speed = 0;
    double big_y_speed = 0;
    double velocity_increase_factor = 1.2;

    while (not quit_requested())
    {
        process_events();

        if (key_typed(C_KEY))
        {
            circle_colour = random_color();
        }

        if (key_down(RIGHT_KEY))
        {
            x += 3;
        }

        if (key_down(LEFT_KEY))
        {
            x -= 3;
        }

        if (x + RADIUS > screen_width())
        {
            x = screen_width() - RADIUS;
        }

        if (x - RADIUS < 0)
        {
            x = RADIUS;
        }

        if (key_down(UP_KEY))
        {
            y -= 3;
        }

        if (key_down(DOWN_KEY))
        {
            y += 3;
        }

        if (y + RADIUS > screen_height())
        {
            y = screen_height() - RADIUS;
        }

        if (y - RADIUS < 0)
        {
            y = RADIUS;
        }

        // update the small circle's position
        small_x1 += small_x_speed1;
        small_y1 += small_y_speed1;

        // check if the small circle hit the right or left wall
        if (small_x1 + SMALL_RADIUS > screen_width()) {
            small_x_speed1 = -small_x_speed1;
            small_x1 = screen_width() - SMALL_RADIUS;
        } else if (small_x1 - SMALL_RADIUS < 0) {
            small_x_speed1 = -small_x_speed1;
            small_x1 = SMALL_RADIUS;
        }

        // check if the small circle hit the top or bottom wall
        if (small_y1 + SMALL_RADIUS > screen_height()) {
            small_y_speed1 = -small_y_speed1;
            small_y1 = screen_height() - SMALL_RADIUS;
        } else if (small_y1 - SMALL_RADIUS < 0) {
            small_y_speed1 = -small_y_speed1;
            small_y1 = SMALL_RADIUS;
        }

        // update the second small circle's position
        small_x2 += small_x_speed2;
        small_y2 += small_y_speed2;

        // check if the second small circle hit the right or left wall
        if (small_x2 + SMALL_RADIUS > screen_width()) {
            small_x_speed2 = -small_x_speed2;
            small_x2 = screen_width() - SMALL_RADIUS;
        } else if (small_x2 - SMALL_RADIUS < 0) {
            small_x_speed2 = -small_x_speed2;
            small_x2 = SMALL_RADIUS;
        }

        // check if the second small circle hit the top or bottom wall
        if (small_y2 + SMALL_RADIUS > screen_height()) {
            small_y_speed2 = -small_y_speed2;
            small_y2 = screen_height() - SMALL_RADIUS;
        } else if (small_y2 - SMALL_RADIUS < 0) {
            small_y_speed2 = -small_y_speed2;
            small_y2 = SMALL_RADIUS;
        }

       if (circles_collide(x, y, RADIUS, small_x1, small_y1, SMALL_RADIUS))
        {
            resolve_collision(x, y, big_x_speed, big_y_speed, small_x1, small_y1, small_x_speed1, small_y_speed1, RADIUS, SMALL_RADIUS);
            small_x_speed1 *= velocity_increase_factor;
            small_y_speed1 *= velocity_increase_factor;
        }

        if (circles_collide(x, y, RADIUS, small_x2, small_y2, SMALL_RADIUS))
        {
            resolve_collision(x, y, big_x_speed, big_y_speed, small_x2, small_y2, small_x_speed2, small_y_speed2, RADIUS, SMALL_RADIUS);
            small_x_speed2 *= velocity_increase_factor;
            small_y_speed2 *= velocity_increase_factor;
        }
    
        clear_screen(COLOR_WHITE);

        // Check for collisions and play the sound effect if there is a collision
        bool small_ball1_colliding = circles_collide(x, y, RADIUS, small_x1, small_y1, SMALL_RADIUS);
        if (small_ball1_colliding && !small_ball1_in_collision)
        {
            play_sound_effect(collision_sound);
        }
        small_ball1_in_collision = small_ball1_colliding;

        bool small_ball2_colliding = circles_collide(x, y, RADIUS, small_x2, small_y2, SMALL_RADIUS);
        if (small_ball2_colliding && !small_ball2_in_collision)
        {
            play_sound_effect(collision_sound);
        }
        small_ball2_in_collision = small_ball2_colliding;

        // draw the big circle
        fill_circle(circle_colour, x, y, RADIUS);

        // draw the first small circle
        fill_circle(COLOR_RED, small_x1, small_y1, SMALL_RADIUS);

        // draw the second small circle
        fill_circle(COLOR_BLUE, small_x2, small_y2, SMALL_RADIUS);

        refresh_screen(60);
        delay(5);
    }

    // Free the bitmap resources
    free_sound_effect(collision_sound);

    return 0;
}