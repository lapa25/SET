from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd

def draw_metric(df, dataset_name, metric, ylabel, filename_part):
    current = df[df["dataset"] == dataset_name]

    plt.figure(figsize=(11, 6))

    for algorithm in current["algorithm"].unique():
        part = current[current["algorithm"] == algorithm]

        plt.plot(
            part["size"],
            part[metric],
            marker="o",
            linewidth=1.5,
            label=algorithm,
        )

    plt.title(f"{dataset_name}: {ylabel}")
    plt.xlabel("Размер массива")
    plt.ylabel(ylabel)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    output_dir = Path("plots")
    output_dir.mkdir(exist_ok=True)

    plt.savefig(output_dir / f"{dataset_name}_{filename_part}.png", dpi=200)
    plt.close()


def main():
    df = pd.read_csv("results.csv")

    for dataset_name in df["dataset"].unique():
        draw_metric(
            df,
            dataset_name,
            "time_us",
            "Среднее время, мкс",
            "time",
        )

        draw_metric(
            df,
            dataset_name,
            "char_comparisons",
            "Количество посимвольных сравнений / обращений",
            "comparisons",
        )

    print("Графики сохранены в папку plots")


if __name__ == "__main__":
    main()