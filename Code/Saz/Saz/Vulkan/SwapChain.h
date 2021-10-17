#pragma once

#include <Saz/Vulkan/Device.h>

// vulkan headers
#include <Vulkan/vulkan.h>

namespace vulkan {

class SwapChain {
 public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  SwapChain(vulkan::Device& deviceRef, ivec2 windowExtent);
  SwapChain(vulkan::Device& deviceRef, ivec2 windowExtent, std::shared_ptr<SwapChain> previous);
  ~SwapChain();

  SwapChain(const SwapChain &) = delete;
  void operator=(const SwapChain &) = delete;

  VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
  VkRenderPass getRenderPass() { return renderPass; }
  VkImageView getImageView(int index) { return swapChainImageViews[index]; }
  size_t imageCount() { return swapChainImages.size(); }
  VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
  ivec2 getSwapChainExtent() { return swapChainExtent; }
  uint32_t width() { return swapChainExtent.x; }
  uint32_t height() { return swapChainExtent.y; }

  float extentAspectRatio() {
    return static_cast<float>(swapChainExtent.x) / static_cast<float>(swapChainExtent.y);
  }
  VkFormat findDepthFormat();

  VkResult acquireNextImage(uint32_t *imageIndex);
  VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

  bool compareSwapFormats(const SwapChain& swapChain) const;

 private:
    void Init();
  void createSwapChain();
  void createImageViews();
  void createDepthResources();
  void createRenderPass();
  void createFramebuffers();
  void createSyncObjects();

  // Helper functions
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const DynamicArray<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const DynamicArray<VkPresentModeKHR> &availablePresentModes);
  ivec2 chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VkFormat swapChainImageFormat;
  ivec2 swapChainExtent;

  DynamicArray<VkFramebuffer> swapChainFramebuffers;
  VkRenderPass renderPass;

  DynamicArray<VkImage> depthImages;
  DynamicArray<VkDeviceMemory> depthImageMemorys;
  DynamicArray<VkImageView> depthImageViews;
  DynamicArray<VkImage> swapChainImages;
  DynamicArray<VkImageView> swapChainImageViews;

  vulkan::Device& device;
  ivec2 windowExtent;

  VkSwapchainKHR swapChain;
  std::shared_ptr<SwapChain> m_OldSwapChain;

  DynamicArray<VkSemaphore> imageAvailableSemaphores;
  DynamicArray<VkSemaphore> renderFinishedSemaphores;
  DynamicArray<VkFence> inFlightFences;
  DynamicArray<VkFence> imagesInFlight;
  size_t currentFrame = 0;
};

}  // namespace lve
