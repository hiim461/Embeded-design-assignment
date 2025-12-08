from django.db import models

class SensorReading(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    
    # Sensor 1
    temp = models.FloatField(null=True)
    press = models.FloatField(null=True)
    alt = models.FloatField(null=True)
    
    # Sensor 2
    IAQ = models.FloatField(null=True)
    TVOC = models.FloatField(null=True)
    eCO2 = models.FloatField(null=True)
    EtOH = models.FloatField(null=True)

    class Meta:
        ordering = ['-timestamp']

    def __str__(self):
        return f"{self.timestamp} | Sensor1: {self.temp}, {self.press}, {self.alt} | Sensor2: {self.IAQ}, {self.TVOC}, {self.eCO2}, {self.EtOH}"



class RawUART(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    raw = models.TextField()

    class Meta:
        ordering = ['-timestamp']

    def __str__(self):
       
        ts = self.timestamp.strftime("%Y-%m-%d %H:%M:%S")
        excerpt = (self.raw[:80] + '...') if len(self.raw) > 80 else self.raw
        return f"[{ts}] {excerpt}"
