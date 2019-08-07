const dot = document.querySelector('.slider__dot'),
      slider = document.querySelector('#r1'),
      sliderReg = document.querySelector('.slider__reg'),
      cordsBlock = document.querySelector('.slider__cords');
let len = 0;    

slider.addEventListener('input', function(){
    cordsBlock.textContent = this.value;
});


