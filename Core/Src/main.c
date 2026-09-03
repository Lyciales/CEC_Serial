/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "serial.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum {
    APP_STATE_IDLE,
    APP_STATE_WAIT_TX
} AppState_t;

AppState_t app_state = APP_STATE_IDLE;
uint32_t delay_start_time = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  RS485_Init(&huart1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // /* USER CODE END WHILE */

    // /* USER CODE BEGIN 3 */
    // // Thêm các biến static để lưu trữ trạng thái và dữ liệu điều khiển giữa các chu kỳ while(1)
    // static uint8_t is_data_zero = 1;
    // static uint8_t target_state = 0;
    // static float target_current = 0.0f; 

	  // switch (app_state) {
		// case APP_STATE_IDLE:
		//   if (frame_received_flag == 1)
		//   {
		// 	// Kiểm tra nghiêm ngặt: Địa chỉ có đúng là SLAVE_ID không? Command = 0x10, Length = 5
		// 	if (valid_frame_buffer.address == SLAVE_ID && 
    //             valid_frame_buffer.command == 0x10 && 
    //             valid_frame_buffer.length == 5)
		// 	{
		// 	  // Kiểm tra xem 5 byte data có bằng 0x00 hết không
		// 	  is_data_zero = 1;
		// 	  for (int i = 0; i < 5; i++) {
		// 		if (valid_frame_buffer.payload[i] != 0x00) {
		// 		  is_data_zero = 0;
		// 		  break;
		// 		}
		// 	  }

		// 	  // Chỉ thực thi mã phân tích điều khiển NẾU địa chỉ đã khớp ở lệnh IF phía trên
		// 	  // và có byte khác 0x00
		// 	  if (is_data_zero == 0) {
		// 		// 1. Tách 1 byte đầu tiên thành trạng thái On/Off
		// 		target_state = valid_frame_buffer.payload[0];
				
		// 		// 2. Bỏ qua 2 byte tiếp theo (payload[1] và payload[2]) do không điều khiển nhiệt độ
				
		// 		// 3. Tách 2 byte cuối thành biến float (chia 100 để tương ứng định dạng %.2f)
		// 		uint16_t current_raw = (valid_frame_buffer.payload[3] << 8) | valid_frame_buffer.payload[4];
		// 		target_current = (float)current_raw / 100.0f; 
		// 	  }

		// 	  delay_start_time = HAL_GetTick(); // Bắt đầu đếm giờ 5ms
		// 	  app_state = APP_STATE_WAIT_TX;    // Chuyển sang trạng thái chờ gửi
		// 	}
		// 	else
		// 	{
		// 	  frame_received_flag = 0; // Xóa cờ từ chối xử lý nếu không đúng ID hoặc sai chuẩn
		// 	}
		//   }
		//   break;

		// case APP_STATE_WAIT_TX:
		//   // Đợi 5ms không chặn CPU
		//   if ((HAL_GetTick() - delay_start_time) >= 5)
		//   {
		// 	uint8_t resp[5];

		// 	if (is_data_zero == 1) {
		// 	  // --- TRƯỜNG HỢP 1: Yêu cầu cập nhật dữ liệu (Data = 0) ---
		// 	  uint8_t  state   = 1;     // 1 byte On/Off state 
		// 	  uint16_t temp    = 2850;  // 2 byte Nhiệt độ
		// 	  uint16_t current = 1205;  // 2 byte Dòng điện
			  
		// 	  resp[0] = state;                      
		// 	  resp[1] = (temp >> 8) & 0xFF;         
		// 	  resp[2] = temp & 0xFF;                
		// 	  resp[3] = (current >> 8) & 0xFF;      
		// 	  resp[4] = current & 0xFF;             
		// 	} 
		// 	else {
		// 	  // --- TRƯỜNG HỢP 2: Yêu cầu cài đặt/điều khiển (Có byte != 0) ---
		// 	  // Gán byte đầu tiên phản hồi thành Slave ID của thiết bị 
		// 	  resp[0] = valid_frame_buffer.payload[0];
			  
		// 	  // Lặp lại (echo) 4 byte còn lại từ bản tin vừa nhận
		// 	  resp[1] = valid_frame_buffer.payload[1];
		// 	  resp[2] = valid_frame_buffer.payload[2];
		// 	  resp[3] = valid_frame_buffer.payload[3];
		// 	  resp[4] = valid_frame_buffer.payload[4];
		// 	}

		// 	// Gửi bản tin qua hàm DMA của serial.c với địa chỉ là SLAVE_ID
		// 	RS485_Send_Frame_DMA(&huart1, SLAVE_ID, 0x90, resp, 5);

		// 	frame_received_flag = 0; // Đặt lại cờ, sẵn sàng cho bản tin tiếp theo
		// 	app_state = APP_STATE_IDLE;
		//   }
		//   break;
	  // }

    // THUẬT TOÁN ĐANG CHẠY THỰC TẾ
    static uint8_t is_data_zero = 1;
    static uint8_t target_state = 0;
    static float target_current = 0.0f; 

    switch (app_state) {
      case APP_STATE_IDLE:
        if (frame_received_flag == 1)
        {
          // Bước 1: Kiểm tra đúng địa chỉ thiết bị (SLAVE_ID)
          if (valid_frame_buffer.address == SLAVE_ID) 
          {
            // --- CASE 1: Lệnh GHI điều khiển (0x10) - Bắt buộc Length = 5 ---
            if (valid_frame_buffer.command == CMD_WRITE_CONTROL && valid_frame_buffer.length == 5)
            {
              // Kiểm tra xem 5 byte data có bằng 0x00 hết không
              is_data_zero = 1;
              for (int i = 0; i < 5; i++) {
                if (valid_frame_buffer.payload[i] != 0x00) {
                  is_data_zero = 0;
                  break;
                }
              }

              // Chỉ phân tích NẾU có byte khác 0x00
              if (is_data_zero == 0) {
                target_state = valid_frame_buffer.payload[0];
                uint16_t current_raw = (valid_frame_buffer.payload[3] << 8) | valid_frame_buffer.payload[4];
                target_current = (float)current_raw / 100.0f; 
              }

              delay_start_time = HAL_GetTick(); 
              app_state = APP_STATE_WAIT_TX;    
            }
            // --- CASE 2: Lệnh ĐỌC trạng thái (0x03) - Bắt buộc Length = 0 ---
            else if (valid_frame_buffer.command == CMD_READ_STATUS && valid_frame_buffer.length == 0)
            {
              // Lệnh đọc không có payload, chỉ cần chuyển thẳng sang trạng thái phản hồi
              delay_start_time = HAL_GetTick(); 
              app_state = APP_STATE_WAIT_TX;
            }
            else
            {
              // Khung tin chứa command lạ hoặc sai độ dài
              frame_received_flag = 0; 
            }
          }
        else
        {
          // Khung tin gửi cho địa chỉ khác
          frame_received_flag = 0; 
        }
      }
      break;

    case APP_STATE_WAIT_TX:
      // Đợi 5ms không chặn CPU
      if ((HAL_GetTick() - delay_start_time) >= 5)
      {
        uint8_t resp[5];

        // --- PHẢN HỒI CHO LỆNH ĐỌC (Mã gửi về: 0x83) ---
        if (valid_frame_buffer.command == CMD_READ_STATUS)
        {
          uint8_t  state   = 1;     // 1 byte On/Off state 
          uint16_t temp    = 2850;  // 2 byte Nhiệt độ
          uint16_t current = 1205;  // 2 byte Dòng điện
          
          resp[0] = state;                      
          resp[1] = (temp >> 8) & 0xFF;         
          resp[2] = temp & 0xFF;                
          resp[3] = (current >> 8) & 0xFF;      
          resp[4] = current & 0xFF;  

          // Gửi bản tin với mã phản hồi RESP_READ_STATUS (0x83)
          RS485_Send_Frame_DMA(&huart1, SLAVE_ID, RESP_READ_STATUS, resp, 5);
        }
        // --- PHẢN HỒI CHO LỆNH GHI (Mã gửi về: 0x90) ---
        else if (valid_frame_buffer.command == CMD_WRITE_CONTROL)
        {
          if (is_data_zero == 1) {
            // Trường hợp cập nhật dữ liệu (Data = 0)
            uint8_t  state   = 1;     
            uint16_t temp    = 2850;  
            uint16_t current = 1205;  
            
            resp[0] = state;                      
            resp[1] = (temp >> 8) & 0xFF;         
            resp[2] = temp & 0xFF;                
            resp[3] = (current >> 8) & 0xFF;      
            resp[4] = current & 0xFF;             
          } 
          else {
            // Trường hợp echo lại thông số cài đặt
            resp[0] = valid_frame_buffer.payload[0];
            resp[1] = valid_frame_buffer.payload[1];
            resp[2] = valid_frame_buffer.payload[2];
            resp[3] = valid_frame_buffer.payload[3];
            resp[4] = valid_frame_buffer.payload[4];
          }
          // Gửi bản tin với mã phản hồi RESP_WRITE_CONTROL (0x90)
          RS485_Send_Frame_DMA(&huart1, SLAVE_ID, RESP_WRITE_CONTROL, resp, 5);
        }
        frame_received_flag = 0; // Đặt lại cờ, sẵn sàng cho bản tin tiếp theo
        app_state = APP_STATE_IDLE;
      }
      break;
    }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// Callback của ST sẽ gọi hàm của thư viện chúng ta
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) { 
        RS485_RxEvent_Handler(huart, Size);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        RS485_TxCplt_Handler();
    }
}

// BỔ SUNG: Cứu hộ tự động khi có nhiễu đường truyền làm treo UART
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // Xóa các cờ lỗi phần cứng
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
        
        // Hủy quá trình hiện tại và khởi động lại DMA Rx
        HAL_UART_AbortReceive(huart);
        HAL_UARTEx_ReceiveToIdle_DMA(huart, rxDataBuffer, RX_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */