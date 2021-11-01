#include "cli_wrap.h"
#include "config.h"
#include "data.h"
#include "log.h"
#include <thread>

#include "random.h"

const std::string CLIENT_ID{"[simple random location data generator]"};

Data get_data()
{
    static Rand rng;
    Data d;

    d.tag_id = rng.next(config::MAX_TAG_AMOUNT);
    d.point.x = rng.next(config::MAX_MEASUREMENT_UNIT);
    d.point.y = rng.next(config::MAX_MEASUREMENT_UNIT);
    d.point.z = rng.next(config::MAX_MEASUREMENT_UNIT);

    return d;
}

void worker(std::atomic_bool& do_work)
{
    MQCli client(MQCli::Type::PUB, CLIENT_ID);

    if (not client.is_valid())
        return;

    while (do_work)
    {
        auto data = get_data();
        lg::info("Sending message...\t{}\t", data.to_string());
        if (not client.publish(data))
        {
            return;
        }

        lg::info("...OK\n");

        std::this_thread::sleep_for(config::WAIT_FOR);
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
