import os
import math
import pandas as pd
import matplotlib.pyplot as plt

def ensure_dir(d):
    os.makedirs(d, exist_ok=True)

def main():
    print("Построение графиков HyperLogLog")
    run_csv = input("Введите путь к run.csv (например data/run.csv): ").strip()
    summary_csv = input("Введите путь к summary.csv (например data/summary.csv): ").strip()
    outdir = input("Введите папку для сохранения результатов (например report/baseline): ").strip()

    stream_id = int(input("Введите stream_id для графика №1 (например 0): ").strip())
    p_str = input("Введите p (если хотите взять из summary, просто напишите -1): ").strip()
    p = int(p_str)

    ensure_dir(outdir)

    run = pd.read_csv(run_csv)
    summ = pd.read_csv(summary_csv)

    # График 1: сравнение N_t и F0^t для одного потока
    s = run[run["stream_id"] == stream_id].copy()
    s = s.sort_values("step")

    x = s["prefix_len"].values
    y_true = s["true_F0"].values
    y_est = s["estimate_N"].values

    plt.figure()
    plt.plot(x, y_true, label="Истинное F0^t")
    plt.plot(x, y_est, label="Оценка N_t (HLL)")
    plt.xlabel("Размер обработанного префикса (t)")
    plt.ylabel("Число уникальных элементов")
    plt.title(f"Поток {stream_id}: истинное значение и оценка HyperLogLog")
    plt.legend()
    fig1 = os.path.join(outdir, "fig1_stream.png")
    plt.savefig(fig1, dpi=160, bbox_inches="tight")
    plt.close()

    # График 2: статистики оценки по всем потокам
    summ = summ.sort_values("step")
    x2 = summ["prefix_len"].values
    m_est = summ["est_mean_N"].values
    s_est = summ["est_std_N"].values
    m_true = summ["true_mean_F0"].values

    plt.figure()
    plt.plot(x2, m_true, label="Среднее истинное F0^t")
    plt.plot(x2, m_est, label="E(N_t)")
    plt.fill_between(x2, m_est - s_est, m_est + s_est, alpha=0.25, label="E(N_t) ± σ(N_t)")
    plt.xlabel("Размер обработанного префикса (t)")
    plt.ylabel("Число уникальных элементов")
    plt.title("HyperLogLog: среднее и стандартное отклонение по потокам")
    plt.legend()
    fig2 = os.path.join(outdir, "fig2_mean_std.png")
    plt.savefig(fig2, dpi=160, bbox_inches="tight")
    plt.close()

    # Сравнение с теорией
    if p == -1:
        p = int(summ["p"].iloc[0])

    m = 2 ** p
    theo_104 = 1.04 / math.sqrt(m)
    theo_132 = 1.32 / math.sqrt(m)

    #эмпирическая относительная дисперсия: sigma(N)/E(N) по шагам
    rel_std = (summ["est_std_N"] / summ["est_mean_N"]).replace([math.inf, -math.inf], float("nan"))
    rel_std_mean = float(rel_std.mean(skipna=True))
    rel_std_max = float(rel_std.max(skipna=True))

    #средняя абсолютная относительная ошибка среднего (смещение)
    rel_err_mean_abs = float((summ["rel_err_mean"].abs()).mean())

    analysis_path = os.path.join(outdir, "analysis.txt")
    with open(analysis_path, "w", encoding="utf-8") as f:
        f.write(f"p={p}, m={m}\n")
        f.write(f"Теория (отн. σ) ~ 1.04/sqrt(m) = {theo_104:.6f}\n")
        f.write(f"Консервативно      1.32/sqrt(m) = {theo_132:.6f}\n\n")
        f.write("Эмпирика (по шагам, усреднение по потокам):\n")
        f.write(f"  среднее sigma(N)/E(N) = {rel_std_mean:.6f}\n")
        f.write(f"  максимум sigma(N)/E(N) = {rel_std_max:.6f}\n")
        f.write(f"  среднее |(E(N)-E(F))/E(F)| ≈ {rel_err_mean_abs:.6f}\n\n")
        f.write("Интерпретация:\n")
        f.write("  Если эмпирическая sigma(N)/E(N) близка к 1.04/sqrt(m), реализация соответствует теории HLL.\n")
        f.write("  Значение 1.32/sqrt(m) — более грубая (завышенная) оценка сверху.\n")

    print("\nГотово, сохранено:")
    print(" ", fig1)
    print(" ", fig2)
    print(" ", analysis_path)

if __name__ == "__main__":
    main()
