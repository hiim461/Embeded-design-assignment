from django.urls import path
from . import views

urlpatterns = [
    path('', views.home, name='home'),
    path('api/latest/', views.api_latest, name='api_latest'),
    path('api/raw/', views.api_raw, name='api_raw'),
]
