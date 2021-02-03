import time
import asyncio
import random

async def syncWaitFun( index ):
    print( 'syncWaitFun ', index, ' start....')
    #await time.sleep( random.random() * 10 )
    await asyncio.sleep( random.random() * 10 )
    print( 'syncWaitFun ', index, ' exit!.....')
    return 'syncWaitFun-Result - %d'%(index,)

async def asyncFun( index ):
    print( 'asyncFun ', index, ' start....')
    result = await syncWaitFun( index )
    print( 'asyncFun ', index, ' result is ', result )
    print( 'asyncFun ', index, '  exit!!')

loop = asyncio.get_event_loop()

tasks = []
for i in range(0,100):
    tasks.append( asyncFun(i) )

loop.run_until_complete( asyncio.gather( *tasks ) )
loop.close()