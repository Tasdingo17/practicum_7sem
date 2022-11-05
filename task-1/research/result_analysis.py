import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

pd.set_option('display.float_format', lambda x: '%.4f' % x)

def read_file_to_pd(filename: str) -> pd.DataFrame:
    with open(filename, 'r') as file:
        d = dict()
        for line in file.readlines():
            line = line[:-1]
            columns = line.split(',')
            for col in columns:
                name, val = col.split(':')
                d.setdefault(name, []).append(val)
        data = pd.DataFrame.from_dict(d)

    for col in data:
        data[col] = pd.to_numeric(data[col])
    return data


def analyze_time(filename: str, resfile: str = "time_heatmap.png"):
    """Draw a heatmap works/proc/time and save it to resfile"""
    data = read_file_to_pd(filename)
    mean_res = data.loc[:,['processors', 'works', "duration_sec"]].groupby(["processors", "works"]).mean().reset_index()
    plt.figure(figsize=(15, 15))

    plt.title("Algorithm excecution time for CAUCHY law, sec", fontsize=14)
    tmp = mean_res.pivot(index='processors', columns='works', values='duration_sec')
    sns.heatmap(tmp, annot=True, cmap='viridis')
    plt.ylabel('Processors Number')
    plt.xlabel('Works Number')

    plt.savefig(resfile)


def analyze_law(filename: str, resfile: str = "law_heatmap.png"):
    """Draw heatmaps for excecution time and relative res increase of different laws,
       save it to resfile"""
    data = read_file_to_pd(filename)
    mean_res = data.groupby(["processors", "works", "law"]).mean().reset_index()
    mean_res["rel_increase_percent"] = ((mean_res["startCriterion"] - mean_res["bestCriterion"]) / 
                                         mean_res["startCriterion"]) * 100
    mean_res["proc-works"] = mean_res['processors'].astype(str) + "-" + mean_res["works"].astype(str)

    rel_res = mean_res.pivot(index='proc-works', columns='law', values='rel_increase_percent')
    time_res = mean_res.pivot(index='proc-works', columns='law', values='duration_sec')

    fig, ax = plt.subplots(1, 2, figsize=(20, 8))
    fig.suptitle('Law comparison on "big" data', fontsize=18)

    sns.heatmap(time_res, annot=True, fmt=".2f", cmap='viridis', ax=ax[0], linewidths=0.25, 
                xticklabels=["Boltzmann", "Cauchy", "Log"])
    ax[0].set_title("Excecution time, sec", fontsize=16)

    sns.heatmap(rel_res, annot=True, fmt=".2f", cmap='plasma', ax=ax[1], linewidths=0.25,
                xticklabels=["Boltzmann", "Cauchy", "Log"])
    ax[1].set_title("Relative result increase, %", fontsize=16)

    for i in range(2):
        ax[i].set_ylabel('Processors-Works', fontsize=14)
        ax[i].set_xlabel('Law', fontsize=14)

    plt.savefig(resfile)


if __name__ == "__main__":
    analyze_time("time_research_res.csv")
    analyze_law("law_research_res.csv")