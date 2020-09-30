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

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].NET_PW_GetJiaoChengStates') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].NET_PW_GetJiaoChengStates					
GO

----------------------------------------------------------------------
CREATE PROC NET_PW_GetJiaoChengStates
	@dwNewsID		INT,				--	���ű�ʶ
	@dwUserID		INT	,			--	�û���ʶ		
	@strErrorDescribe	NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

BEGIN
	-- ��������
	SET NOCOUNT ON
	DECLARE @DanJian INT
	--DECLARE @JiaoChengQuanXianID1 INT
	select @DanJian=jiaoChengRMB from news where newsID=@dwNewsID
	if @DanJian=0
	begin --����Ϊ0��Ϊ���ѧϰ��
		SET @strErrorDescribe=N'2'
		return 2
	END
		
	IF  EXISTS ( select * FROM JiaoChengQuanXian where (NewsID=@dwNewsID AND UserID=@dwUserID ))	
	begin--������������
		IF  EXISTS ( select * FROM JiaoChengQuanXian where (NewsID=@dwNewsID AND UserID=@dwUserID and isShengHe=1))
		BEGIN --�Ѿ�����˿���ѧϰ
			SET @strErrorDescribe=N'2'
			return 2	
		end
		else
		begin--�����˵�û���		
			SET @strErrorDescribe=N'1'
			return 1	
		end
	end
	else--û����		
	begin 
		SET @strErrorDescribe=N'0'      
		return 0  
	end

END

