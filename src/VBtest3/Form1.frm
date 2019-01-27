VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3030
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   4560
   LinkTopic       =   "Form1"
   ScaleHeight     =   3030
   ScaleWidth      =   4560
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   855
      Left            =   1080
      TabIndex        =   0
      Top             =   600
      Width           =   1935
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Declare Function JT_Layouts Lib "NewUI.dll" (ByVal path As String) As Integer

   ' Private Declare Function B_Function1 Lib "B1.dll" (ByVal a As Integer, ByVal b As Integer) As Integer
   'Private Declare Function A_Function1 Lib "A1.dll" (ByVal a As Integer, ByVal b As Integer) As Integer
    'Private Declare Function Function1 Lib "NewUI.dll" (ByVal a As Integer, ByVal b As Integer) As Integer

Private Sub Command1_Click()
    
   ' Dim c As Integer
     '   c = A_Function1(1, 3)
    
    
    Dim result As Integer
   Dim path As String
    
    path = "E:\\袁梓埠个人文件夹\\代码\\PictureLayout\\src\\VBtest3\\真实测试.xml"
    result = JT_Layouts(path)
    
    MsgBox "ok"
    
End Sub
