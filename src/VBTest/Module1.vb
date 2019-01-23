



Module Module1


    'Public Declare Function Function1 Lib "NewUId.dll" (ByVal a As Integer, ByVal b As Integer) As Integer
    'Public Declare Function JT_Layouts Lib "NewUId.dll" (ByVal src_file_path As String) As Integer


    Public Declare Function Function1 Lib "NewUI.dll" (ByVal a As Integer, ByVal b As Integer) As Integer
    Public Declare Function JT_Layouts Lib "NewUI.dll" (ByVal src_file_path As String) As Integer


    Sub Main()
        ' Dim c As Integer
        ' c = Function1(1, 3)


        Dim s As String

        s = "真实测试.xml"
        's = "F:\\PictureLayout\\真实测试.xml"
        ' s = "E:\\袁梓埠个人文件夹\\代码\\PictureLayout\\真实测试_家里电脑.xml"
        's = "G:\\真实测试.xml"
        JT_Layouts(s)

        MsgBox("Hello World")

    End Sub

End Module
