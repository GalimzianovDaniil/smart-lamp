const addBtn = document.querySelector('.add-device__btn'),
      deviceNames = document.querySelector('.device__names'),
      confirmBtn = document.querySelector('.devices__confirm');

addBtn.addEventListener('click', function() {
    const deviceBlock = document.createElement('div');
    deviceBlock.classList.add('device');
    deviceBlock.innerHTML = `
        <h3 class="device__title">Устройство</h3>
        <form id="form__device">
            <input type="text" id="ssid" class="form-text"><label for="ssid" class="form-text">Имя девайса</label>
            <input type="password" id="password" class="form-text"><label for="ssid" class="form-text">Пароль</label>
        </form>
        <button class="delete-device">Удалить</button>
    `;
    deviceNames.appendChild(deviceBlock);
    const deleteBtn = deviceBlock.querySelector('.delete-device');
        deleteBtn.addEventListener('click', function(){
            deviceBlock.remove();
    });
});

confirmBtn.addEventListener('click', function(){
    const inputAutoSsid = document.querySelector('#form__main-device > #ssid'),
          inputAutoPass = document.querySelector('#form__main-device > #password'),
          inputsDevicesSsid = document.querySelectorAll('#form__device > #ssid'),
          inputsDevicesPass = document.querySelectorAll('#form__device > #password');
    let mainJson = {},
        devicesJson = {},
        counter = 0;
    inputsDevicesSsid.forEach(function(key, i){
        devicesJson[key.value] = inputsDevicesPass[i].value;
    });
    mainJson.ssid = inputAutoSsid.value;
    mainJson.password = inputAutoPass.value;

    let xhr = new XMLHttpRequest(),
        request ='/init?';
    for (let key in mainJson) {
        request += `${key}=${mainJson[key]}&`
    }
    request += `len=${inputsDevicesSsid.length}&`
    for (let key in devicesJson) {
        counter++;
        request += `device${counter}=${key}&password${counter}=${devicesJson[key]}&`;
    }
    console.log(request.slice(0, -2));
    xhr.open('GET', request.slice(0, -2));
    xhr.send();
});