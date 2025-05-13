一、開發環境 : 
作業系統 : windows 
使用語言 : python 
測試環境 : CPU : Intel Core i5-7300HQ 
Visual Studio Code 
二、實作方法和流程 : 
main :  
先讀入檔名、k和對應的檔案，再分別執行方法一到方法四。 
方法一 : 
將讀入的檔案丟進Mission1，透過BubbleSort()完成排序並輸出結果。 
方法二 : 
將讀入的檔案丟進Mission2，先將存input的list切成k份，一起存於
同一個list中，並將此list設為全域變數，接著使用threading.thread()
呼叫k個threads，並在第i個thread中對第i份資料做Bubble sort，
等待所有threads完成後，再做merge sort，每當要進行merge sort
時，就呼叫一個thread並將參數設為要進行merge的兩筆資料在list
中的位置，將merge出來的結果存入另一個全域的temp list中，當list
中的資料都merge過一遍後，將list的內容改為temp list的內容，並
清空temp list，再重呼叫thread對list 的資料作merge，直到list中只
剩下一筆merge完的資料便完成，再將結果輸出。 
方法三 : 
先將存input的list切成k份，一起存於同一個list中，接著使用
multiprocessing.Process()呼叫 k 個 process，並在第 i 個 process 中對第 i
份資料做Bubble sort，因為子process中對變數的值做修改會影響到父
process 中該變數的值，所以不用將list設為全域，可以直接將要排序
的資料丟給子process執行，等待所有process完成後，做完後再對頭
兩筆資料做merge sort，將結果放入list的最後面，將兩筆資料
remove()，再重複以上步驟，直到list中只剩下一筆資料便完成，再將
結果輸出。 
方法四 : 
先將存input的list切成k份，一起存於同一個list中，接著輪流對每
一筆資料做bubble sort，做完後再對頭兩筆資料做merge sort，將結果
放入list 的最後面，將兩筆資料remove()，再重複以上步驟，直到list
中只剩下一筆資料便完成，再將結果輸出。 