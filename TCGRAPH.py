import pandas as pd
import numpy as np
import datetime
import matplotlib.pyplot as plt


def TCGRAPH(outputpath, Tradingday=252):
    def outputpathCheck(outputpath):
        if outputpath == "":
            raise ValueError("outputpath is empty")
        return True

    def TradingdayCheck(Tradingday):
        if Tradingday == "":
            raise ValueError("Tradingday is empty")
        return True

    outputpathCheck(outputpath)
    TradingdayCheck(Tradingday)

    G_table = pd.read_excel(outputpath + "\\LPPL_table.xlsx")
    G = G_table.values

    num_table = pd.read_excel(outputpath + "\\pricedata.xlsx", sheet_name="Sheet1", usecols="A:B")
    Price = num_table["price"].values
    Date = num_table["date"].values
    t = pd.to_datetime(Date)

    LogPrice = np.log(Price)

    tc_history = np.sort(G[:, 2])

    FIGURE = plt.figure()

    P = LogPrice

    if len(G) > 28:
        num = 20
    elif len(G) > 2:
        per = 0.7
        num = len(tc_history) * per
    else:
        per = 1
        num = len(tc_history) * per

    nelements, centers = np.histogram(tc_history, num)
    pelements = nelements / np.sum(nelements)

    centers1 = centers
    pelements1 = pelements

    t_centers1 = (centers1 - len(t)) * 365 / Tradingday + t[len(t)-1]

    plt.plot(t, P)
    plt.ylabel('Log price', fontsize=17)

    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()
    ax2.bar(t_centers1, pelements1, color='g', edgecolor='g')

    FIGURE.savefig(outputpath + "\\TCGRAPH_figure.jpg", dpi=1000)
    plt.close(FIGURE)

    colnames = ['Centers', 'Relative Frequency']
    IntialValue_table = pd.DataFrame(data={'Centers': centers1, 'Relative Frequency': pelements1})
    IntialValue_table.to_excel(outputpath + "\\TCGRAPH_table.xlsx", index=False, columns=colnames)


# 함수 사용 예시
#output_path = "경로를 입력하세요"
#TCGRAPH(output_path)
