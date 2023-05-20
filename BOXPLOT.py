import pandas as pd
import matplotlib.pyplot as plt
import datetime
from matplotlib.dates import date2num
from matplotlib.ticker import FuncFormatter
import os

def boxplot(outputpath, Tradingday):
    # read tables from excel files
    G_table = pd.read_excel(os.path.join(outputpath, 'LPPL_table.xlsx'))
    num_table = pd.read_excel(os.path.join(outputpath, 'pricedata.xlsx'), sheet_name='Sheet1', usecols='A')

    # convert to datetime
    t = pd.to_datetime(num_table['date'])

    Tc = G_table['Tc']
    # Assuming Tc is in years and t[0] is in datetime format
    Tc_datetime = pd.to_timedelta(Tc * 365 / Tradingday, unit='D') + t.iloc[0]
    Tc_num = date2num(Tc_datetime)

    fonts = 17
    fig, ax = plt.subplots()

    # boxplot
    ax.boxplot(Tc_num, vert=False, labels=[' '])

    ax.set_ylabel('Box plot', fontsize=fonts)

    # set the date format
    formatter = FuncFormatter(lambda x, pos: datetime.datetime.fromordinal(int(x)).strftime('%Y.%m'))
    ax.xaxis.set_major_formatter(formatter)

    plt.savefig(os.path.join(outputpath, 'BOXPLOT_figure.jpg'), dpi=1000)
    plt.close(fig)

