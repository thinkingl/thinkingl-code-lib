import smartserver

smartTxt = open( 'test.txt' ).read()

transedTxt = smartserver.transSMARTInfo( smartTxt )

print( transedTxt )

open( 'trans.txt', 'w' ).write( transedTxt )