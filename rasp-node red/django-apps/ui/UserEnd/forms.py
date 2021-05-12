from django import forms

from .models import Item

class FormItemForm4(forms.ModelForm):
    name = forms.CharField(label='name', max_length=50)
    make = forms.CharField(label='maker', max_length=50)
    model = forms.CharField(label='model', max_length=50)
    color = forms.CharField(label='color', max_length=50)
    mac = forms.CharField(label='mac', max_length=50)


class FormItemForm(forms.ModelForm):
    class Meta:
        model = Item
        fields = ("name", "make", "model", "color", "mac")

