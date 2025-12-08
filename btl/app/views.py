from django.shortcuts import render
from django.http import JsonResponse
from .models import SensorReading, RawUART 
from django.utils import timezone
import datetime


def home(request):
    return render(request, 'app/home.html')

def api_latest(request):
    latest = SensorReading.objects.order_by('-timestamp').first()
    if not latest:
        return JsonResponse({'ok': False})
    data = {
       'ok': True,
        'temp': latest.temp,
        'press': latest.press,
        'alt': latest.alt,
        'IAQ': latest.IAQ,
        'TVOC': latest.TVOC,
        'eCO2': latest.eCO2,
        'EtOH': latest.EtOH,
        'timestamp': latest.timestamp.isoformat()
    }
    return JsonResponse(data)


def api_raw(request):
    raws = list(RawUART.objects.order_by('-timestamp')[:10])[::-1]
    out = []
    for r in raws:
        ts = r.timestamp
        # Nếu naive (không có timezone), giả sử nó đang ở UTC và làm aware
        if timezone.is_naive(ts):
            ts = timezone.make_aware(ts, timezone.utc)
        # giờ ts là timezone-aware; gửi ISO cho client xử lý
        out.append({
            "timestamp_iso": ts.isoformat(),
            "raw": r.raw
        })
    return JsonResponse({"ok": True, "lines": out})