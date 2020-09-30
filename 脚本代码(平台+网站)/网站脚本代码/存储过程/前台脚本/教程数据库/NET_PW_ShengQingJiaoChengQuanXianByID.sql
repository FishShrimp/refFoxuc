----------------------------------------------------------------------
-- ��Ȩ��2013
-- ʱ�䣺2013-02-20
-- ��;���������̳�Ȩ��
----------------------------------------------------------------------
USE QPNativeWebDB
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_ShengQingJiaoChengQuanXianByID') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_ShengQingJiaoChengQuanXianByID					
GO

----------------------------------------------------------------------
CREATE PROC NET_PW_ShengQingJiaoChengQuanXianByID
	@dwNewsID		INT,				--	���ű�ʶ
	@dwUserID		INT	,			--	�û���ʶ	
	@ShengQingIP    varchar(20),--IP
	@strErrorDescribe	NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

BEGIN
	-- ��������
	SET NOCOUNT ON
	DECLARE @JiaoChengQuanXianID INT
	--DECLARE @JiaoChengQuanXianID1 INT
		
	IF  EXISTS ( select * FROM JiaoChengQuanXian where (NewsID=@dwNewsID AND UserID=@dwUserID ))	
	begin--������������
		--SELECT @JiaoChengQuanXianID1=JiaoChengQuanXianID FROM JiaoChengQuanXian where (NewsID=@dwNewsID AND UserID=@dwUserID and isShengHe=0)
		--IF (@JiaoChengQuanXianID1!=null)--�����˵�û������		
		SET @strErrorDescribe=N'�Ѿ����룬����������'
		return 1	
	end
	else		
	begin 
		INSERT INTO [QPNativeWebDB].[dbo].[JiaoChengQuanXian]
               ([NewsID],[UserID],[ShengQingDate],[ShengQingIP])          
		VALUES (@dwNewsID,@dwUserID,getdate(),@ShengQingIP)
		SET @strErrorDescribe=N'����ɹ�'        
		return 0  
	end

END

