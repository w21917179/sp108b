if
======
    if (條件式) 
    {
       // 當條件式為真實這裡會執行
    }

執行結果  
------

輸入:  

    a = 3;  
    b = 5;  
    if (a > b)  
        t = a;  
    else  
        t = b;  
        
輸出:  

      t0 = 3
      a = t0
      t1 = 5
      b = t1
      t2 = a
      t3 = b
      t4 = t2 > t3
      if not T4 goto L0
      t5 = a
      t = t5
      goto L1
      (L0)
      t6 = b
      t = t6
      (L1)
      
for
======
      for ( 啟始值; 條件式; 更新值 ) {
         statement(s); //當條件是為真實這裡會執行，並執行更新值
      }
      
執行結果      
------

輸入:

    a=0;
    for(i=1; i<3; i=i+1){
        a=a+1;
    }
    
輸出:

    t0 = 0
    a = t0
    t1 = 1
    i = t1
    (L0)
    t2 = i
    t3 = 3
    t4 = t2 < t3
    if T4 goto L2
    goto L3
    (L1)
    t5 = i
    t6 = 1
    t7 = t5 + t6
    i = t7
    goto L0
    (L2)
    t8 = a
    t9 = 1
    t10 = t8 + t9
    a = t10
    goto L1
    (L3)
    
goto
======
    goto label;
    ..
    .
    label: statement;  
    //當goto label時，會自動跳到"label:"這行
      
執行結果      
------

輸入:

    banana:
    goto world;
    apple:
    goto banana;
    world:
    goto apple;
    
輸出:

    (L0)
    goto L1
    (L2)
    goto L0
    (L1)
    goto L2
    
參考資料
======
if,for,goto是由原程式擴充
其中使用的函數如下來自原程式

    nextLabel()
    skip()
    isNext()
    isNextNext() -> 由isNext()改寫
    STMT()
    BLOCK();
    ASSIGN()
    ASSIGNfor() -> 由ASSIGN()改寫


原程式
<https://github.com/ccccourse/sp2/blob/master/compiler/03-compiler/compiler.c>
