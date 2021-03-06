VipList = {}

-- private variables
local vipWindow = nil

-- public functions
function VipList.create()
  vipWindow = UI.display('viplist.otui', { parent = Game.gameRightPanel })
end

function VipList.destroy()
  vipWindow:destroy()
  vipWindow = nil
end

-- hooked events
function Game.onAddVip(id, name, online)
  local vipList = vipWindow:getChildById('vipList')

  local label = UILabel.create()
  vipList:addChild(label)
  label:setId('vip' .. id)
  label:setText(name)
  label:setStyle('VipListLabel')

  if online then
    label:setForegroundColor('#00ff00')
  else
    label:setForegroundColor('#ff0000')
  end

  label.vipOnline = online
end

function Game.onVipStateChange(id, online)
  local vipList = vipWindow:getChildById('vipList')
  local label = vipList:getChildById('vip' .. id)

  if online then
    label:setForegroundColor('#00ff00')
  else
    label:setForegroundColor('#ff0000')
  end

  label.vipOnline = online
end

connect(Game, { onLogin = VipList.create,
                onLogout = VipList.destroy })
