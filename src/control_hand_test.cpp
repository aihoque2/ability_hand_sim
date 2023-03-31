#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/float64_multi_array.hpp"

class JointPub : public rclcpp::Node
{
public:
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr pub;
    std::shared_ptr<std_msgs::msg::Float64MultiArray> msg;
    rclcpp::TimerBase::SharedPtr timer;
    int idx; // idx for picking which joint to move
    // double sign = 1.0; // used for testing individual joints

    JointPub() : Node("test_hand_controls")
    {
        using namespace std::chrono_literals;

        pub = this->create_publisher<std_msgs::msg::Float64MultiArray>("joint_effort_controller/commands", 10);
        timer = this->create_wall_timer(500ms, std::bind(&JointPub::timer_callback, this));

        msg = std::make_shared<std_msgs::msg::Float64MultiArray>();
        msg->data = std::vector<double>(10, 0.0);

        idx = 0;
    }

    void timer_callback()
    {
        if (idx == 10)
            idx = 0;
        msg->data[idx] = 0.15;
        pub->publish(*msg);
        msg->data[idx] = 0.0;
        // sign *= -1.0;
        idx++;
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JointPub>());
    rclcpp::shutdown();
    return 0;
}