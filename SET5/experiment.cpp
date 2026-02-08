#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <cstdint>
#include <algorithm>

#include "random_stream_gen.h"
#include "hash_func_gen.h"
#include "hyperloglog.h"
#include "hyperloglog_packed.h"

//среднее
static double mean(const std::vector<double>& x) {
    double s = 0.0;
    for (const double v : x) {
        s += v;
    }
    return x.empty() ? 0.0 : s / static_cast<double>(x.size());
}

//выборочное стандартное отклонение
static double stdev_sample(const std::vector<double>& x) {
    if (x.size() < 2) {
        return 0.0;
    }
    const double m = mean(x);
    double s2 = 0.0;
    for (const double v : x) {
        const double d = v - m;
        s2 += d * d;
    }
    return std::sqrt(s2 / static_cast<double>(x.size() - 1));
}

int main() {
    //ввод параметров
    std::size_t num_streams;
    std::size_t stream_len;
    std::size_t universe_size;
    std::size_t steps;
    int p_int;
    uint64_t seed;
    uint64_t hash_seed;
    int packed_int;
    std::string out_csv;
    std::string summary_csv;

    std::cout << "Введите num_streams: ";
    std::cin >> num_streams;

    std::cout << "Введите stream_len: ";
    std::cin >> stream_len;

    std::cout << "Введите universe_size: ";
    std::cin >> universe_size;

    std::cout << "Введите steps: ";
    std::cin >> steps;

    std::cout << "Введите p (m = 2^p): ";
    std::cin >> p_int;
    auto p = static_cast<uint8_t>(p_int);

    std::cout << "Введите seed (для генератора потока): ";
    std::cin >> seed;

    std::cout << "Введите hash_seed (для HashFuncGen): ";
    std::cin >> hash_seed;

    std::cout << "Packed версия? (0 - нет, 1 - да): ";
    std::cin >> packed_int;
    bool packed = (packed_int != 0);

    std::cout << "Имя файла out_csv (например run.csv): ";
    std::cin >> out_csv;

    std::cout << "Имя файла summary_csv (например summary.csv): ";
    std::cin >> summary_csv;

    const uint32_t m_regs = 1u << p;
    const std::vector<std::size_t> prefixes = RandomStreamGen::prefix_lengths(stream_len, steps);
    const std::string algo_name = packed ? "packed" : "baseline";

    std::cerr << "Запуск: " << (packed ? "HyperLogLogPacked" : "HyperLogLog") << "\n";
    std::cerr << "m=" << m_regs << ", шагов=" << prefixes.size() << "\n";

    //csv для записи резов
    std::ofstream out(out_csv);
    out << "algo,stream_id,step,prefix_len,true_F0,estimate_N\n";

    //для статистик по шагам: собираем значения по всем потокам
    std::vector<std::vector<double>> est_per_step(prefixes.size());
    std::vector<std::vector<double>> true_per_step(prefixes.size());
    for (std::size_t i = 0; i < prefixes.size(); ++i) {
        est_per_step[i].reserve(num_streams);
        true_per_step[i].reserve(num_streams);
    }

    //цикл по потокам
    for (std::size_t sid = 0; sid < num_streams; ++sid) {
        //генератор потока
        RandomStreamGen::Params sp;
        sp.stream_len = stream_len;
        sp.universe_size = universe_size;
        sp.seed = seed + sid * 1315423911ull; //разные seed на разные потоки

        RandomStreamGen gen(sp);

        //хеш-функция
        HashFuncGen hf(hash_seed);

        //поток как индексы из универсума
        auto ind_stream = gen.generate_stream_indices();

        //точный подсчёт уникальных: множество индексов
        std::unordered_set<uint32_t> seen;
        seen.reserve(std::min<std::size_t>(universe_size, stream_len));

        //гиперлоглог структуры
        HyperLogLog hll(p);
        HyperLogLogPacked hllp(p);

        std::size_t step_id = 0;

        for (std::size_t i = 0; i < ind_stream.size(); ++i) {
            uint32_t uidx = ind_stream[i];
            seen.insert(uidx);

            const std::string& s = gen.universe().at(uidx);
            uint32_t h = hf(s);

            if (!packed) {
                hll.add_hash(h);
            } else {
                hllp.add_hash(h);
            }

            if (step_id < prefixes.size() && (i + 1) == prefixes[step_id]) {
                std::size_t pref_len = prefixes[step_id];
                auto trueF0 = static_cast<double>(seen.size());
                double estN = packed ? hllp.estimate() : hll.estimate();

                out << algo_name << "," << sid << "," << step_id
                    << "," << pref_len << "," << static_cast<uint64_t>(trueF0) << "," << estN << "\n";

                true_per_step[step_id].push_back(trueF0);
                est_per_step[step_id].push_back(estN);

                ++step_id;
            }
        }
    }

    out.close();

    //итоговые резы
    std::ofstream sum(summary_csv);
    sum << "algo,step,prefix_len,p,m,true_mean_F0,true_std_F0,est_mean_N,est_std_N,rel_err_mean\n";

    for (std::size_t step = 0; step < prefixes.size(); ++step) {
        double mTrue = mean(true_per_step[step]);
        double sTrue = stdev_sample(true_per_step[step]);
        double mEst  = mean(est_per_step[step]);
        double sEst  = stdev_sample(est_per_step[step]);

        double rel = (mTrue > 0) ? (mEst - mTrue) / mTrue : 0.0;

        sum << algo_name << "," << step << "," << prefixes[step]
            << "," << static_cast<int>(p) << "," << m_regs
            << "," << mTrue << "," << sTrue
            << "," << mEst << "," << sEst
            << "," << rel << "\n";
    }

    sum.close();

    //печать памяти под регистры
    if (!packed) {
        HyperLogLog tmp(p);
        std::cerr << "Память (baseline, регистры): " << tmp.memory_bytes() << " байт\n";
    } else {
        HyperLogLogPacked tmp(p);
        std::cerr << "Память (packed, регистры): " << tmp.memory_bytes() << " байт\n";
    }

    std::cerr << "Готово. CSV: " << out_csv << " и " << summary_csv << "\n";
}
