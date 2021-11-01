#include "cli_wrap.h"
#include "config.h"
#include "data.h"
#include "log.h"
#include <atomic>
#include <thread>

#include <cfenv> // feclearexcept
#include <cmath> // hypot

constexpr auto CLIENT_ID{"[location data processor]"};

std::atomic_bool work_on{true};

class Proc
{
  public:
    double calculate_distance(const Data& data)
    {
        const auto tag_id = data.tag_id;
        const auto point = data.point;

        const auto last_point = std::exchange(last_data_map_[tag_id], point);

        // clear errors
        errno = 0;
        std::feclearexcept(FE_ALL_EXCEPT);

        // calculate the distance
        auto distance = std::hypot(point.x - last_point.x, point.y - last_point.y, point.z - last_point.z);
        if (errno == ERANGE)
        {
            lg::error("{}\n", std::strerror(errno));
            return 0.0;
        }
        return distance;
    }

  private:
    std::map<TagId_t, Point> last_data_map_; // tagID to last point map
    Point last_point_;
};

void worker(std::atomic_bool& do_work)
{
    MQCli client(MQCli::Type::SUB, CLIENT_ID);

    if (not client.is_valid())
        return;

    Proc proc;

    while (do_work)
    {
        auto data = client.consume_message<Data>();
        if (not data.has_value())
            continue;

        lg::info("{}\t", data->to_string());

        auto distance = proc.calculate_distance(data.value());
        lg::info("distance: {}\n", distance);
    }
}

int main(int argc, char* argv[])
{
    std::atomic_bool do_work{true};

    auto worker_thread = std::thread(worker, std::ref(do_work));

    while (auto ch = std::getchar())
    {
        if (ch == 'q') // stop on 'q'
        {
            do_work = false;
            break;
        }
    }

    worker_thread.join();

    lg::info("\nExiting\n");
    return 0;
}
