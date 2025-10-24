#include "fingerprint.h"
#include "bsp_finger.h"
#include "cmsis_os.h"

extern osThreadId defaultTaskHandle;
extern gui::Window wn_fg_authen;
extern gui::Window wn_fg_pass;
extern gui::Window wn_fg_fail;

static FingerprintResult PollAuthencation();
static FingerprintResult PollEnroll();

static volatile bool on_running = false;
static FingerprintRequest* lateset_req = nullptr;
static TaskHandle_t fg_task_handle;
static uint32_t fg_stack[128];
static StaticTask_t fg_task;

void Fingerprint::Authen(FingerprintRequest* req)
{
    on_running = true;
    lateset_req = req;
    req->dis->switchFocusLag(&wn_fg_authen);
    req->dis->refresh_count = 11;
    GUI_OP_NULL();
    fg_task_handle = xTaskCreateStatic([](void * argument) -> void
    {
        auto* req = static_cast<FingerprintRequest*>(argument);
        auto rtvl = PollAuthencation();
        if (rtvl == FG_PASS)
        {
            req->dis->switchFocusLag(&wn_fg_pass);
        }
        else
        {
            req->dis->switchFocusLag(&wn_fg_fail);
        }
        req->dis->refresh_count = 11;
        GUI_OP_NULL();
        if (req->result_callback != nullptr)
            req->result_callback(rtvl, req);
        on_running = false;
        vTaskDelete(nullptr);
    }, "fg_task", 128, req, osPriorityNormal, fg_stack, &fg_task);
}

void Fingerprint::Enroll(FingerprintRequest* req)
{
    on_running = true;
    lateset_req = req;
    req->dis->switchFocusLag(&wn_fg_authen);
    req->dis->refresh_count = 11;
    GUI_OP_NULL();
    fg_task_handle = xTaskCreateStatic([](void * argument) -> void
    {
        auto* req = static_cast<FingerprintRequest*>(argument);
        auto rtvl = PollEnroll();
        if (rtvl == FG_PASS)
        {
            req->dis->switchFocusLag(&wn_fg_pass);
        }
        else
        {
            req->dis->switchFocusLag(&wn_fg_fail);
        }
        req->dis->refresh_count = 11;
        GUI_OP_NULL();
        if (req->result_callback != nullptr)
            req->result_callback(rtvl, req);
        on_running = false;
        vTaskDelete(nullptr);
    }, "fg_task", 128, req, osPriorityNormal, fg_stack, &fg_task);
}

void Fingerprint::clearAllFinger(FingerprintRequest* req)
{
    on_running = true;
    lateset_req = req;
    fg_task_handle = xTaskCreateStatic([](void * argument) -> void
    {
        auto* req = static_cast<FingerprintRequest*>(argument);
        FIN_EN_GPIO_Port->BSRR = FIN_EN_Pin;
        osDelay(800);
        auto rtvl = finger::clearChar();
        FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
        FingerprintResult result = FG_ERROR;
        if (rtvl == 0)
        {
            result = FG_PASS;
            req->dis->switchFocusLag(&wn_fg_pass);
        }
        else
        {
            req->dis->switchFocusLag(&wn_fg_fail);
        }
        req->dis->refresh_count = 11;
        GUI_OP_NULL();
        if (req->result_callback != nullptr)
            req->result_callback(result, req);
        on_running = false;
        vTaskDelete(nullptr);
    }, "fg_task", 128, req, osPriorityNormal, fg_stack, &fg_task);
}


void Fingerprint::cancelOperation()
{
    if (on_running)
    {
        vTaskDelete(fg_task_handle);
        on_running = false;
    }
    if (lateset_req != nullptr)
    {
        lateset_req->dis->switchFocusLag(lateset_req->wn_src);
        lateset_req->dis->refresh_count = 0;
        GUI_OP_NULL();
        lateset_req = nullptr;
    }
}

FingerprintResult PollAuthencation()
{
    uint8_t resp;
    //NOLINTNEXTLINE
    finger::SearchResult result;
    FIN_EN_GPIO_Port->BSRR = FIN_EN_Pin;
    osDelay(800);
    for (;;)
    {
        while (HAL_GPIO_ReadPin(FIN_WKP_GPIO_Port, FIN_WKP_Pin) != GPIO_PIN_SET)
        {
            osDelay(10);
        }
        resp = finger::getImageAuth();
        if (resp == 1)
        {
            // Module Failure
            goto failure;
        }
        if (resp == 2) continue;
        if (resp == 0) break;
    }
    resp = finger::genChar();
    if (resp == 1)
    {
        // Module Failure
        goto failure;
    }
    if (resp != 0)
    {
        // Bad image
        FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
        return FG_BadImage;
    }
    result = finger::Search();
    if (result.result == 1)
    {
        // Module Failure
        goto failure;
    }
    if (result.result == 0)
    {
        // Pass
        FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
        return FG_PASS;
    }
    if (result.result == 9)
    {
        FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
        return FG_FAIL;
    }
    if (result.result != 0)
    {
        // Bad image
        FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
        return FG_BadImage;
    }
    failure:
    FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
    return FG_ERROR;
}

FingerprintResult PollEnroll()
{
    uint8_t resp;
    FIN_EN_GPIO_Port->BSRR = FIN_EN_Pin;
    osDelay(800);
    for (uint8_t buffer_id = 1;;)
    {
        while (HAL_GPIO_ReadPin(FIN_WKP_GPIO_Port, FIN_WKP_Pin) != GPIO_PIN_SET)
        {
            osDelay(10);
        }
        resp = finger::getImageEnroll();
        if (resp == 1)
        {
            // Module Failure
            goto failure;
        }
        if (resp == 2) continue;
        if (resp == 0)
        {
            resp = finger::genChar(buffer_id);
            if (resp == 1)
            {
                // Module Failure
                goto failure;
            }
            if (resp == 0)
            {
                buffer_id++;
            }
        }
        if (buffer_id == 4)
        {
            break;
        }
        // Release finger
        while (HAL_GPIO_ReadPin(FIN_WKP_GPIO_Port, FIN_WKP_Pin) == GPIO_PIN_SET)
        {
            osDelay(10);
        }
    }
    resp = finger::regModel();
    if (resp != 0)
    {
        // Module Failure
        goto failure;
    }
    finger::deleteChar(0);
    finger::storeChar(0);
    FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
    return FG_PASS;
    failure:
    FIN_EN_GPIO_Port->BRR = FIN_EN_Pin;
    return FG_ERROR;
}
