import Vue from 'vue'
import axios from 'axios'

import App from './App'
import router from './router'
import store from './store'
import AgoraRtcEngine from '../../..'

if (!process.env.IS_WEB) Vue.use(require('vue-electron'))
Vue.http = Vue.prototype.$http = axios
Vue.config.productionTip = false

/* eslint-disable no-new */

console.log('before AgoraRtcEngine')
let engine = new AgoraRtcEngine()
console.log('after new AgoraRtcEngine')
engine.initialize('kwk4')
console.log('after initialize')

new Vue({
  components: { App },
  router,
  store,
  template: '<App/>'
}).$mount('#app')
