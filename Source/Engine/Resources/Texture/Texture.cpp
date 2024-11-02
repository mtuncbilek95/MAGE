#include "Texture.h"

#include "../ControlUnit/TransferBatch.h"
#include "../TexLoader/TexLoader.h"

namespace MAGE
{
	Texture::Texture() : m_context(Gfx::Context::Get()), m_props({})
	{
		m_transferBuffer = TransferBatch::Get().GetTransferPool()->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY);
	}

	Texture::~Texture()
	{
		Destroy();
	}

	void Texture::CreateImage(const String& imagePath)
	{
		TexData data = Importer::LoadTexture(imagePath);
		
		StageBufferProps props = {};
		props.sizeInBytes = data.channelCount * data.imageSize.x * data.imageSize.y;

		m_buffer = MakeOwned<VStageBuffer>(props, m_context.GetDevice());
		m_buffer->MapMemory(data.data);

		m_props.imageSize = data.imageSize;
		m_props.channelCount = data.channelCount;
	}

	void Texture::TransferData()
	{
		RecordingProps recProp = {};
		recProp.beginFlag = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		m_transferBuffer->BeginRecording(recProp);
		
		printf("Start\n");
		ImageBarrierProps barrierProp =
		{
			.srcPipeline = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			.dstPipeline = VK_PIPELINE_STAGE_TRANSFER_BIT,

			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = &*m_image,

			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		};

		m_transferBuffer->TranslateImageBarrier(barrierProp);
		m_transferBuffer->CopyBufferToImage(&*m_buffer, &*m_image);
		
		printf("End\n");

		barrierProp.srcPipeline = VK_PIPELINE_STAGE_TRANSFER_BIT,
		barrierProp.dstPipeline = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		barrierProp.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrierProp.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrierProp.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrierProp.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		m_transferBuffer->TranslateImageBarrier(barrierProp);
		m_transferBuffer->EndRecording();

		TransferBatch::Get().RegisterCmdBuffer(&*m_transferBuffer);
	}

	void Texture::Destroy()
	{
		m_buffer->Destroy();
		m_image->Destroy();
		m_view->Destroy();
	}
}
