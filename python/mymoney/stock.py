import akshare as ak


if __name__=="__main__":
    qfq_factor_df = ak.stock_zh_a_daily(symbol="sz000002", adjust="qfq-factor")
    print(qfq_factor_df)