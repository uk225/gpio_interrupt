#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

static const struct of_device_id my_device_match[] = {
    { .compatible = "org,custom-gpio" },
    { }
};
MODULE_DEVICE_TABLE(of, my_device_match);
struct gpio_desc *led_gpio;
static int status = 1;

static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
    pr_info("Interrupt fired!\n");
    gpiod_set_value(led_gpio,(status++)%2);

    return IRQ_HANDLED;
}

static int gpio_probe(struct platform_device *pdev)
{
    struct gpio_desc *irq_gpio;
    int irq, ret;

    irq_gpio = devm_gpiod_get(&pdev->dev, "irq", GPIOD_IN);
    if (IS_ERR(irq_gpio))
        return PTR_ERR(irq_gpio);

    led_gpio = devm_gpiod_get(&pdev->dev,"user-led",GPIOD_OUT_LOW);
    if(IS_ERR(led_gpio))
    {
        return PTR_ERR(led_gpio);
    }
    pr_info("user led is detected\n");



    irq = gpiod_to_irq(irq_gpio);
    if (irq < 0)
        return irq;

    ret = devm_request_irq(&pdev->dev, irq, my_irq_handler,
                           IRQF_TRIGGER_FALLING,
                           "gpio_irq", &pdev->dev);
    if (ret)
        return ret;

    pr_info("GPIO IRQ registered: %d\n", irq);
    return 0;
}

static int gpio_remove(struct platform_device *pdev)
{
    return 0;
}

static struct platform_driver gpio_driver = {
    .probe = gpio_probe,
    .remove = gpio_remove,
    .driver = {
        .name = "custom-gpio",
        .of_match_table = my_device_match,
    },
};

module_platform_driver(gpio_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("GPIO interrupt driver");